#include "subcommands.h"
#include "utils.h"
#include "lib/extlibs.h"

#include <libgen.h>
#include <stdio.h>
#include <fcntl.h>
#include <limits.h>
#include <time.h>

#include <string.h>

#define IO_BUFF_SIZE 262144 // 256 KiB

void move_cwd(const struct dl_entry * entry) {
  char * base = basename(entry->path);

  fs_move_file(entry->path, base, IO_BUFF_SIZE);
}

void copy_cwd(const struct dl_entry * entry) {
  char * base = basename(entry->path);

  fs_copy_file(entry->path, base, IO_BUFF_SIZE);
}

void stream_stdout(char * path) {
  int fd = open_or_die(path, O_RDONLY);
  
  fd_copy_all(fd, STDOUT_FILENO, IO_BUFF_SIZE);
  
  if(-1 == close(fd))
    die("Failed to close %s", path);
}

void time_since_last_access(time_t access_time, char * buf, size_t size) {
  time_t sec_delta = time(NULL) - access_time;

  struct time_delta delta;
  get_time_delta(&delta, sec_delta); 

  if(delta.days >= 365) {
    struct tm * time_stamp = localtime(&access_time);
    strftime(buf, size, "%c", time_stamp);
  } else if(delta.days > 0) {
    snprintf(
      buf, size,
      "%d "REG_GREEN"days"STY_RESET", %d "REG_GREEN"hours"STY_RESET" ago",
      delta.days,
      delta.hours
    );
  } else if(delta.hours > 0) {
    snprintf(
      buf, size, 
      "%d "REG_GREEN"hours"STY_RESET", %d "REG_GREEN"minutes"STY_RESET" ago",
      delta.hours,
      delta.mins
    );
  } else if(delta.mins > 0) {
    snprintf(
      buf, size,
      "%d "REG_GREEN"minutes"STY_RESET", %d "REG_GREEN"seconds"STY_RESET" ago", 
      delta.mins,
      delta.secs
    );
  } else {
    snprintf(buf, size, "%d "REG_GREEN"seconds"STY_RESET" ago", delta.secs);
  }
}

size_t max_name_len(struct dl_stack * stack) {
  size_t max = 0,
         len;

  for (int i = 0; i < stack->size; ++i) {
    len = strlen(stack->entries[i]->name);
    max = MAX(len, max);
  }

  return max;
}

void dls_list(struct dls_args * args, struct dl_stack * stack) {
  char time_buf[100];

  size_t padding_len = max_name_len(stack) + 1;

  char * name_buf = alloca(padding_len);

  struct dl_entry * entry;
  for (ssize_t i = stack->size - 1; i >= 0; i--) {
    entry = stack->entries[i];

    time_since_last_access(entry->access_time, time_buf, sizeof(time_buf));

    leftpad(name_buf, padding_len, entry->name);
    
    printf("#%zd  "REG_WHITE"%s"STY_RESET"   %s\n", i, name_buf, time_buf); 
  }
}

unsigned long validate_depth(struct dls_args * args, struct dl_stack * stack) {
  unsigned long wanted = args->pos.depth;
  
  if(wanted == MAX_DEPTH) {
    wanted = stack->size; 
  } else if(wanted > stack->size) {
    die(
        "Can not %s %lu entries. Only %lu entries exist on the stack\n",
        dls_cmd_str[args->cmd],
        wanted,
        stack->size
    );
  }

  return wanted;
}

void dls_pop(struct dls_args * args, struct dl_stack * stack) {
  unsigned long depth = validate_depth(args, stack);
  bool stream = args->opts.stream;

  struct dl_entry * entry;
  for (unsigned long i = 0; i < depth; ++i) {
    entry = stack->entries[i];

    if(stream) {
      stream_stdout(entry->path);

      unlink_or_die(entry->path);
    } else {
      move_cwd(entry); 
    }
  }
}

void dls_top(struct dls_args * args, struct dl_stack * stack) {
  unsigned long depth = validate_depth(args, stack);
   
  struct dl_entry * entry;
  for (unsigned long i = 0; i < depth; ++i) {
    entry = stack->entries[i];

    stream_stdout(entry->path);
  }
}

void dls_discard(struct dls_args * args, struct dl_stack * stack) {
  unsigned long depth = validate_depth(args, stack);
  
  for (unsigned long i = 0; i < depth; ++i)
    unlink_or_die(stack->entries[i]->path);
}

void push(const char * path, const char * dl_path) {
  const char * base = basename((char *) path);
  char * downloads_path = pathcat(dl_path, base);

  fs_move_file(path, downloads_path, IO_BUFF_SIZE);
  
  free(downloads_path); 
}

void dls_push(struct dls_args * args, struct config_options * config) {
  if(args->dpush.paths) {

    for (int i = 0; i < args->dpush.size; ++i)
      push(args->dpush.paths[i], config->downloads_path);

  } else {
    push(args->pos.path, config->downloads_path);
  }
}
