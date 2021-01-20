#ifndef __CONFIG_PARSER_H
#define __CONFIG_PARSER_H

struct config_options {
  char * downloads_path;
};

void parse_config(struct config_options * co);

#endif
