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
        goto not_found;
    if (S_ISREG(file_info.st_mode))
        return file;
    else if (S_ISDIR(file_info.st_mode))
        return folder;
    else if (S_ISLNK(file_info.st_mode)){
        int ret = get_file_type(str, stat);
        return ret > 0 ? symbolic + ret : invalid;
    }
not_found:
    (void)printf("ls: cannot access '%s': %s\n", str, strerror(errno)); // RETURN SYMBOLIC INFINITE LOOP GARBAGE TODO
    return invalid;
}

static int8_t parse_args(const struct ls_params *params, struct queue *f_queue, struct queue *d_queue, const int argc)
{
    int ret = SUCCESS;
    if (params->argument_count == 0)
        ret = add_to_queue(".", folder, argc, d_queue);
    for (uint64_t i = 0; i < params->argument_count; i++){
        enum file_type type = get_file_type(params->arguments[i], lstat);
        if (type == invalid)
            continue;
        else if (type == file || type == symbolic_file)
            ret = add_to_queue(params->arguments[i], type, argc, f_queue);
        else if (type == folder || type == symbolic_folder)
            ret = add_to_queue(params->arguments[i], type, argc, d_queue);
        if (ret == FATAL_ERROR)
            break;
    }
    return ret;
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
    format(&f_queue);
    format(&d_queue);
    cleanup(&params, &f_queue, &d_queue);
    return SUCCESS;
}
