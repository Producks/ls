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
#define EXIT_ERROR 1
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
    bool time_sort; // -t for time sort
    bool reverse_sort; // -r reverse sort
    uint64_t argument_count;
    char **arguments;
};

struct file_info{
    char file_name[256];
    struct stat file_stat;
};

struct queue{
    uint16_t count;
    uint16_t size;
    struct file_info **q;
    char parent_name[256];
};

struct tree{
    uint16_t child_count;
    uint16_t childs_size;
    struct tree *parent;
    struct tree **childs;
    struct queue *q;
};

int     ls(int argc, char **argv);
int8_t  fatal_error(void);


int8_t  parse_params(struct ls_params *params, int argc, char**argv);
void    clean_params(struct ls_params *params);


int8_t  add_to_queue(const char *str, const enum file_type type, int argc, struct queue *q);
void    clean_queue(struct queue *q);


void    set_cmp_func(struct ls_params *params);
void    sort(struct queue *q);

void    set_print_func(const struct ls_params *params, const struct queue *d_q, const struct queue *f_q);
void    format(const struct queue *q);

#endif
