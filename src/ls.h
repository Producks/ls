#ifndef LS_H
#define LS_H

#include "../lib/ls_utils.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>

#define FATAL_ERROR -1
#define NOT_FOUND -1
#define SUCCESS 0

enum file_type{
    folder = 1,
    file = 2,
    symbolic = 3,
    symbolic_folder = 4,
    symbolic_file = 5,
    invalid = -1
};

struct ls_params{
    bool long_listing; // -l use a long listing format
    bool recursive; // -R, --recursive list subdirectories recursively
    bool show_hidden; // -a, --all do not ignore entries starting with .
    bool time_sort; // -t for time sort
    bool reverse_sort; // -r reverse sort
    uint64_t argument_count;
    char **arguments;
};

struct file_info{
    char file_name [256];
    struct stat file_stat;
    uint16_t ascii_size;
};

struct file_queue{
    uint16_t count;
    uint16_t size;
    struct file_info **q;
};

int ls(int argc, char **argv);
int8_t fatal_error(void);

int8_t parse_params(int argc, char**argv);
void clean_params(void);

int8_t add_f_queue(const char *str, const enum file_type type);
int8_t create_queue(const char *file, struct file_queue *queue);
void clean_f_queue(void);

#endif
