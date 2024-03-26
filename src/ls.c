#include "ls.h"

bool show_hidden = false;

int8_t fatal_error(void)
{
    perror("ls");
    return FATAL_ERROR;
}

static void cleanup(struct ls_params *params, struct queue *f_queue, struct queue *d_queue)
{
    clean_params(params);
    clean_queue(f_queue);
    clean_queue(d_queue);
}

static enum file_type get_file_type(const char *str, int (*f)(const char *, struct stat *))
{
    struct stat file_info;
    if (f(str, &file_info) == NOT_FOUND)
        goto error;
    if (S_ISREG(file_info.st_mode))
        return file;
    else if (S_ISDIR(file_info.st_mode))
        return directory;
    else if (S_ISLNK(file_info.st_mode)){
        int ret = get_file_type(str, stat);
        return ret > 0 ? symbolic + ret : symbolic_infinite;
    }
    else if (S_ISBLK(file_info.st_mode))
        return blk;
    else if (S_ISCHR(file_info.st_mode))
        return chr;
    else if (S_ISFIFO(file_info.st_mode))
        return fifo;
    else if (S_ISSOCK(file_info.st_mode))
        return socket;
error:
    (void)printf("ls: cannot access '%s': %s\n", str, strerror(errno));
    return invalid;
}

static int8_t parse_args(const struct ls_params *params, struct queue *f_queue, struct queue *d_queue, const int argc)
{
    int ret = SUCCESS;
    if (params->argument_count == 0)
        ret = add_to_fix_queue(".", directory, argc, d_queue);
    for (uint64_t i = 0; i < params->argument_count; i++){
        enum file_type type = get_file_type(params->arguments[i], lstat);
        if (type == invalid)
            continue;
        else if (type == directory || type == symbolic_directory)
            ret = add_to_fix_queue(params->arguments[i], type, argc, d_queue);
        else
            ret = add_to_fix_queue(params->arguments[i], type, argc, f_queue);
        if (ret == FATAL_ERROR)
            break;
    }
    return ret;
}

static void handle_folder(const char *directory)
{
    DIR *result = opendir(directory);
}

static void handle_recursive_folder(void)
{
    return;
}

static void magic(struct queue *f_queue, struct queue *d_queue, const bool recursive)
{
    format(f_queue);
    for (int i = 0; i < d_queue->count; i++){
        if (recursive == false)
            handle_folder(d_queue->q[i]->file_name);
        else
            handle_recursive_folder();
    }
}

int ls(int argc, char **argv)
{
    struct ls_params params = {false, false, false, false, 0, NULL};
    struct queue f_queue = {0, 0, NULL, "\0"};
    struct queue d_queue = {0, 0, NULL, "\0"};

    if (parse_params(&params, argc, argv))
        return EXIT_ERROR;
    if (parse_args(&params, &f_queue, &d_queue, argc))
        return EXIT_ERROR;
    set_cmp_func(&params);
    set_print_func(&params, &d_queue, &f_queue);
    sort(&d_queue);
    sort(&f_queue);
    magic(&f_queue, &d_queue, params.recursive);
    cleanup(&params, &f_queue, &d_queue);
    return SUCCESS;
}
