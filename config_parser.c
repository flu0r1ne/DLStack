#include "./tomlc99/toml.h"
#include "config_parser.h"
#include "utils.h"
#include "lib/extlibs.h"

#include <errno.h>
#include <stdlib.h>

#define TOML_ERRBUF_SIZE 1024
#define LOCATION_DB ".config/dls/config"

const char * get_home_or_die() {
  const char * home = getenv("HOME");

  if(!home)
    die("\"HOME\" environmental variable is not set");

  return home;
}

FILE* get_config_fp() {
  const char * home = get_home_or_die();

  char * loc_db = pathcat(home, LOCATION_DB);

  if(!loc_db)
    die("Error no memory.");

  FILE * fp = fopen(loc_db, "r");  

  free(loc_db);

  return fp; 
}

void populate_defaults(struct config_options * co) {
  const char * home = get_home_or_die();

  co->downloads_path = pathcat(home, "Downloads");
}

void parse_config(struct config_options * co) {
  FILE * fp = get_config_fp();

  if(!fp) {
    errno = 0;
    populate_defaults(co);
    return; 
  }

  char err_buf[TOML_ERRBUF_SIZE];

  toml_table_t * conf = toml_parse_file(fp, err_buf, TOML_ERRBUF_SIZE);

  if(!conf)
    die("Cannot parse configuration file:\n\n %s", err_buf);

  toml_datum_t dl_path = toml_string_in(conf, "downloads_path");

  if(!dl_path.ok)
    die("Downloads location (\"downloads_path\") is not defined in the configuration file\n");

  co->downloads_path = dl_path.u.s;

  toml_free(conf);
  fclose(fp);
}
