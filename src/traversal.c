#include "ls.h"

static struct queue *create_dynamic_queue_recursive(char *path, char *file)
{
    struct queue *q = malloc(sizeof(struct queue));
    if (!q)
        return perror("ls"), NULL;
    q->q = malloc(sizeof(struct file_info *) * 30);
    if (!q->q)
        return perror("ls"), free(q), NULL;
    q->count = 0;
    q->size = 30;
    f_strcpy(q->parent_name, file);
    if (!path){
        uint32_t index = 0;
        for (; file[index]; index++)
            q->path[index] = file[index];
        // q->path[index++] = '/';
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

static void fill_queue_from_directory_recursive(struct queue *q, const char *directory)
{
    DIR *result = opendir(q->path);
    if (!result){
        (void)printf("ls: cannot open directory '%s': %s\n", directory, strerror(errno));
        return;
    }
    struct dirent *dp;
    while ((dp=readdir(result)) != NULL){
        if (is_hidden(dp->d_name))
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

void traversal(struct file_info *dir, char *path)
{
    struct queue *q = create_dynamic_queue_recursive(path, dir->file_name);
    if (!q)
        return;
    fill_queue_from_directory_recursive(q, dir->file_name);
    // sort(q);
    format(q);
    // fflush(stdout);
    for (uint16_t index = 0; index < q->count; index++){
        if (q->q[index]->type == directory)
            traversal(q->q[index], q->path);
    }
    clean_dynamic_queue(&q);
}
