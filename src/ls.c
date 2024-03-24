#include "ls.h"


struct ls_params params = {false, false, false, false, false, 0, NULL};
struct file_queue f_queue = {0, 0, NULL};
// static uint32_t recursion_counter = 0;

int8_t fatal_error(void)
{
    perror("ls");
    return FATAL_ERROR;
}

static void cleanup(void)
{
    clean_params();
    clean_f_queue();
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
    (void)printf("ls: cannot access '%s': %s\n", str, strerror(errno));
    return invalid;
}

int8_t parse_args(void)
{
    int ret = SUCCESS;
    for (uint64_t i = 0; i < params.argument_count; i++){
        enum file_type type = get_file_type(params.arguments[i], lstat);
        if (type == invalid)
            continue;
        else if (type == file || type == symbolic_file)
            ret = add_f_queue(params.arguments[i], type);
        if (ret == FATAL_ERROR)
            break;
    }
    return ret;
}

int ls(int argc, char **argv)
{
    if (parse_params(argc, argv))
        return 1;
    if (parse_args())
        return 1;
    // print_f_q();
    cleanup();
    return SUCCESS;
}
