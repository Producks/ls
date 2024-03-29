#include "ls.h"

bool show_hidden = false;

void clean_queue(struct queue *q)
{
    if (q->q){
        for (int i = 0; i < q->count; i++)
            free(q->q[i]);
        free(q->q);
    }
}

void clean_dynamic_queue(struct queue **q)
{
    if ((*q)->q){
        for (int i = 0; i < (*q)->count; i++)
            free((*q)->q[i]);
        free((*q)->q);
        free((*q));
    }
}

struct queue *create_dynamic_queue(const char *str)
{
    struct queue *q = malloc(sizeof(struct queue));
    if (!q)
        return perror("ls"), NULL;
    q->q = malloc(sizeof(struct file_info *) * 30);
    if (!q->q)
        return perror("ls"), free(q), NULL;
    q->count = 0;
    q->size = 30;
    f_strcpy(q->parent_name, str);
    return q;
}

int8_t add_to_dynamic_queue(struct queue *q, const char *file_path, const char *file_name)
{
    if (q->size == q->count)
        return SUCCESS; // REALLOCATE LATER FUNC NEEDED
    struct file_info *file = create_file_info(file_path, file_name);
    if (!file)
        return FATAL_ERROR;
    q->q[q->count++] = file;
    return SUCCESS;
}

int8_t add_to_fix_queue(int argc, struct queue *q, struct file_info *add)
{
    if (q->size == 0){
        q->q = malloc(sizeof(struct file_info *) * argc);
        if (!q->q)
            return FATAL_ERROR;
        q->size = argc;
    }
    q->q[q->count++] = add;
    return SUCCESS;
}

void fill_queue_from_directory(struct queue *q, const char *directory)
{
    DIR *result = opendir(directory);
    if (!result){
        (void)printf("ls: cannot open directory '%s': %s\n", directory, strerror(errno));
        return;
    }
    struct dirent *dp;
    while ((dp=readdir(result)) != NULL){
        if (is_hidden(dp->d_name) == true && show_hidden == false)
            continue;
        char *file_path = f_separator(directory, dp->d_name, '/');
        if (!file_path)
            break;
        int ret = add_to_dynamic_queue(q, file_path, dp->d_name);
        free(file_path);
        if (ret == FATAL_ERROR)
            break;
    }
    closedir(result);
}

void fill_queue_from_directory_recursive(struct queue *q, const char *directory)
{
    char *file_path = f_separator(q->parent_name, directory, '/');
    DIR *result = opendir(directory);
    if (!result){
        (void)printf("ls: cannot open directory '%s': %s\n", directory, strerror(errno));
        return;
    }
    struct dirent *dp;
    while ((dp=readdir(result)) != NULL){
        if (is_hidden(dp->d_name) == true && show_hidden == false)
            continue;
        // char *file_path = f_separator(directory, dp->d_name, '/');
        // if (!file_path)
        //     break;
        int ret = add_to_dynamic_queue(q, file_path, dp->d_name);
        free(file_path);
        if (ret == FATAL_ERROR)
            break;
    }
    closedir(result);
}
