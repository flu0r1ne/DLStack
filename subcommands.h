#ifndef __SUBCOMMANDS_H
#define __SUBCOMMANDS_H 

#include "arg_parser.h"
#include "dl_stack.h"
#include "config_parser.h"

void dls_list(struct dls_args * args, struct dl_stack * stack);

void dls_pop(struct dls_args * args, struct dl_stack * stack);

void dls_top(struct dls_args * args, struct dl_stack * stack);

void dls_discard(struct dls_args * args, struct dl_stack * stack);

void dls_push(struct dls_args * args, struct config_options * config);

#endif
