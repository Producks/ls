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
#include <time.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>

# ifdef __linux__
    #include <linux/limits.h>
# endif

# ifdef __APPLE__
    typedef mode_t __mode_t;
    #include <limits.h>
# endif

#define FATAL_ERROR -1
#define EXIT_ERROR 1
#define NOT_FOUND -1
#define SUCCESS 0

//https://man7.org/linux/man-pages/man5/dir_colors.5.html
//https://github.com/openbsd/src/blob/master/sys/sys/stat.h SO MANY OUTDATED RESOURCES ONLINE REEE

#define OW_R(m)      ((m) & S_IRUSR)    /* Read permission, owner. */
#define OW_W(m)      ((m) &  S_IWUSR)    /* Write permission, owner. */
#define OW_E(m)      ((m) &  S_IXUSR)    /* Execute/search permission, owner. */

#define G_R(m)       ((m) & S_IRGRP)    /* Read permission, group. */
#define G_W(m)       ((m) & S_IWGRP)    /* Write permission, group. */
#define G_E(m)       ((m) & S_IXGRP)    /* Execute/search permission, group. */

#define OT_R(m)      ((m) & S_IROTH)    /* Read permission, others. */
#define OT_W(m)      ((m) & S_IWOTH)    /* Write permission, others. */
#define OT_E(m)      ((m) & S_IXOTH)    /* Execute/search permission, others. */


enum file_type{
    directory = 1,
    file,
    fifo,
    blk,
    chr,
    socket,
    symbolic,
    symbolic_directory,
    symbolic_file,
    symbolic_fifo,
    symbolic_blk,
    symbolic_chr,
    symbolic_socket,
    broken_sym,
    invalid = -1,
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
    enum file_type type;
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

void    clean_queue(struct queue *q);
struct queue *create_dynamic_queue(const char *str);
int8_t add_to_fix_queue(int argc, struct queue *q, struct file_info *add);
void clean_dynamic_queue(struct queue **q);
int8_t add_to_dynamic_queue(struct queue *q, const char *file_path, const char *file_name);
void fill_queue_from_directory(struct queue *q, const char *directory);

void    set_cmp_func(struct ls_params *params);
void    sort(struct queue *q);

void    set_print_func(const struct ls_params *params, const struct queue *d_q, const struct queue *f_q);
void    format(const struct queue *q);

struct file_info *create_file_info(const char *file_path, const char *file_name);
char *get_real_folder(const char *path);
bool is_hidden(const char *str);
void get_permission(const __mode_t mode, char *buffer);
char get_type(const enum file_type type);
char *get_owner(uid_t id);
char *get_group(uid_t id);

#endif
