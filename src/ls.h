#ifndef LS_H
#define LS_H

#include "../lib/ls_utils.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define FATAL_ERROR -1
#define SUCCESS 0

enum sort_type{
    normal,
    reverse,
    time
};

struct ls_params{
    bool long_listing; // -l use a long listing format
    bool recursive; // -R, --recursive list subdirectories recursively
    bool show_dot; // -a, --all do not ignore entries starting with .
    enum sort_type type; // -r for reverse, -t for sort by time, no parameters sort by ascii values
    uint64_t argument_count;
    char **arguments;
};

int ls(int argc, char **argv);


int8_t parse_params(int argc, char**argv);
void clean_params(void);

#endif