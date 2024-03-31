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
    q->q = malloc(sizeof(struct file_info *) * DYNAMIC_QUEUE_PRE_ALLOC);
    if (!q->q)
        return perror("ls"), free(q), NULL;
    q->count = 0;
    q->size = DYNAMIC_QUEUE_PRE_ALLOC;
    f_strcpy(q->parent_name, str);
    return q;
}

struct queue *create_dynamic_queue_recursive(const char *path, const char *file)
{
    struct queue *q = malloc(sizeof(struct queue));
    if (!q)
        return perror("ls"), NULL;
    q->q = malloc(sizeof(struct file_info *) * DYNAMIC_QUEUE_PRE_ALLOC);
    if (!q->q)
        return perror("ls"), free(q), NULL;
    q->count = 0;
    q->size = DYNAMIC_QUEUE_PRE_ALLOC;
    f_strcpy(q->parent_name, file);
    if (!path){
        uint32_t index = 0;
        for (; file[index]; index++)
            q->path[index] = file[index];
        q->path[index] = '\0';
    }
    else{
        uint32_t index = 0;
        for (; path[index]; index++)
            q->path[index] = path[index];
        q->path[index++] = '/';
        for (uint32_t i = 0; file[i]; i++)
            q->path[index++] = file[i];
        q->path[index] = '\0';
    }
    return q;
}

static int8_t reallocate_dynamic_queue(struct queue *q)
{
    uint32_t new_size = q->size * 2;
    struct file_info **new_q = malloc(sizeof(struct file_info *) * new_size);
    if (!new_q){
        perror("ls");
        return FATAL_ERROR;
    }
    q->size = new_size;
    for (uint32_t i = 0; i < q->count; i++)
        new_q[i] = q->q[i];
    free(q->q);
    q->q = new_q;
    return SUCCESS;
}

int8_t add_to_dynamic_queue(struct queue *q, const char *file_path, const char *file_name)
{
    if (q->size == q->count)
        if (reallocate_dynamic_queue(q))
            return FATAL_ERROR;
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
    DIR *result = opendir(q->path);
    if (!result){
        (void)printf("ls: cannot open directory '%s': %s\n", directory, strerror(errno));
        return;
    }
    struct dirent *dp;
    while ((dp=readdir(result)) != NULL){
        if (is_hidden(dp->d_name) && show_hidden == false)
            continue;
        char *file_path = f_separator(q->path, dp->d_name, '/');
        if (!file_path)
            break;
        int ret = add_to_dynamic_queue(q, file_path, dp->d_name);
        free(file_path);
        if (ret == FATAL_ERROR)
            break;
    }
    closedir(result);
}
