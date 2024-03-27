#include "ls.h"

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

static int8_t parse_args(const struct ls_params *params, struct queue *f_queue, struct queue *d_queue, const int argc)
{
    int ret = SUCCESS;
    if (params->argument_count == 0){
        struct file_info *info = create_file_info(".", ".");
        if (!info)
            return FATAL_ERROR;
        ret = add_to_fix_queue(argc, d_queue, info);
    }
    for (uint64_t i = 0; i < params->argument_count; i++){
        struct file_info *info = create_file_info(params->arguments[i], params->arguments[i]);
        if (!info)
            continue;
        else if (info->type == directory || info->type == symbolic_directory)
            ret = add_to_fix_queue(argc, d_queue, info);
        else
            ret = add_to_fix_queue(argc, f_queue, info);
        if (ret == FATAL_ERROR)
            break;
    }
    return ret;
}


static void handle_folder(struct file_info *directory)
{
    struct queue *q = create_dynamic_queue(directory->file_name);
    if (!q)
        return;
    if (directory->type == symbolic_directory){
        char *real_folder = get_real_folder(directory->file_name);
        fill_queue_from_directory(q, real_folder);
        free(real_folder);
    }
    else
        fill_queue_from_directory(q, directory->file_name);
    sort(q);
    format(q);
    clean_dynamic_queue(&q);
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
            handle_folder(d_queue->q[i]);
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
    if (parse_args(&params, &f_queue, &d_queue, argc)){
        cleanup(&params, &f_queue, &d_queue);
        return EXIT_ERROR;
    }
    set_cmp_func(&params);
    set_print_func(&params, &d_queue, &f_queue);
    sort(&d_queue);
    sort(&f_queue);
    magic(&f_queue, &d_queue, params.recursive);
    cleanup(&params, &f_queue, &d_queue);
    return SUCCESS;
}
