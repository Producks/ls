#include "ls.h"

extern bool show_hidden;

void clean_queue(struct queue *q)
{
    if (q->q){
        for (int i = 0; i < q->count; i++)
            free(q->q[i]);
        free(q->q);
    }
}

// Fixed size queue, won't need to grow larger
int8_t add_to_queue(const char *str, const enum file_type type, int argc, struct queue *q)
{
    (void)type;
    if (*str == '.' && show_hidden == false)
        return SUCCESS;
    if (q->size == 0){
        q->q = malloc(sizeof(struct file_info *) * argc - 1);
        if (!q->q)
            return FATAL_ERROR;
        q->size = argc - 1;
    }
    struct file_info *file = malloc(sizeof(struct file_info));
    if (!file)
        return FATAL_ERROR;
    f_strcpy(file->file_name, str);
    if (stat(str, &file->file_stat) == FATAL_ERROR)
        return free(file), FATAL_ERROR;
    q->q[q->count++] = file;
    return SUCCESS;
}
