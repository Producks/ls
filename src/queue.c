#include "ls.h"

extern struct file_queue f_queue;

void clean_f_queue(void)
{
    if (f_queue.q){
        for (int i = 0; i < f_queue.count; i++)
            free(f_queue.q[i]);
        free(f_queue.q);
    }
}

// Start at 5, and double everytime after that. Not being allowed to use realloc make this my best option for now...
static int8_t allocate_f_queue(void)
{
    if (f_queue.size == 0)
        f_queue.size = 5;
    else
        f_queue.size *= 2;
    struct file_info **queue = malloc(sizeof(struct file_info *) * f_queue.size);
    if (queue == NULL)
        return FATAL_ERROR;
    for (uint16_t count = 0; count < f_queue.count; count++)
        queue[count] = f_queue.q[count];
    if (f_queue.q)
        free(f_queue.q);
    f_queue.q = queue;
    return SUCCESS;
}

int8_t add_f_queue(const char *str, const enum file_type type)
{
    (void)type;
    int ret = SUCCESS;
    if (f_queue.count == f_queue.size){
        ret = allocate_f_queue();
        if (ret == FATAL_ERROR)
            return ret;
    }
    struct file_info *file = malloc(sizeof(struct file_info));
    if (!file)
        return FATAL_ERROR;
    f_strcpy(file->file_name, str);
    ret = stat(str, &file->file_stat);
    if (ret == FATAL_ERROR){
        free(file);
        return ret;
    }
    f_queue.q[f_queue.count++] = file;
    return SUCCESS;
}

int8_t create_queue(const char *file, struct file_queue *queue)
{
    (void)queue;
    DIR *directory = opendir(file);
    if (!directory)
        return fatal_error();
    struct dirent *dp;
    while ((dp=readdir(directory)) != NULL){
        printf("%s\n", dp->d_name);
    }
    closedir(directory);
    return SUCCESS;
}
