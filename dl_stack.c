#include "dl_stack.h"
#include "utils.h"

#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

#include "lib/extlibs.h"

/*
 * Preforms your canonical quicksort on the rows of the suffix array
 */
void quicksort(struct dl_entry ** entries, int low, int high){
  if(high - low < 1)
    return;

  int left = low + 1;
  int right = high;
  while(left < right){
    if(entries[right]->access_time < entries[low]->access_time)
      right--;
    else if(entries[left]->access_time >= entries[low]->access_time)
      left++;
    else
      SWAP(entries[left], entries[right]);
  }

  if(entries[left]->access_time < entries[low]->access_time)
    left--;

  SWAP(entries[left], entries[low]);

  quicksort(entries, low, left - 1);
  quicksort(entries, left + 1, high);
}

struct dl_stack * read_entries(const char * dl_path) {
  int fd = open(dl_path, O_RDONLY);

  if(fd == -1)
    die_errno("Could not open downloads directory \"%s\"", dl_path);

  DIR* dl_dir = fdopendir(fd);

  struct dl_entry ** entries = NULL; 
  size_t nr = 0,
         na = 0;

  struct stat s;
  char * entry_path;

  struct dirent * ent;
  while((ent = readdir(dl_dir)) != NULL) {
    if(errno)
      die_errno("Count not read the content of %s", dl_path);

    entry_path = pathcat(dl_path, ent->d_name);

    if(-1 == stat(entry_path, &s))
      die_errno("Error reading file the attributes of \"%s\"", ent->d_name);

    if(S_ISREG(s.st_mode)) {
      struct dl_entry * new_entry = smalloc(sizeof(struct dl_entry));

      new_entry->name = strdup(ent->d_name);
      new_entry->path = entry_path;
      new_entry->access_time = s.st_atime;

      ALLOC_GROW(entries, nr + 1, na);

      entries[nr++] = new_entry;
    } else {
      free(entry_path);
    }
  }
  
  free(dl_dir);

  struct dl_stack * stack = smalloc(sizeof(struct dl_stack));

  *stack = (struct dl_stack) {
    .entries = entries,
    .size = nr
  };

  return stack;
}

void free_dl_stack(struct dl_stack * stack) {
  for (int i = 0; i < stack->size; ++i) {
    free(stack->entries[i]->path);
    free(stack->entries[i]->name);
    free(stack->entries[i]);
  }

  free(stack->entries);
  free(stack);
}
