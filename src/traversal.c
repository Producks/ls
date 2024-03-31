#include "ls.h"

void traversal(const struct file_info *dir, const char *path)
{
    struct queue *q = create_dynamic_queue_recursive(path, dir->file_name);
    if (!q)
        return;
    fill_queue_from_directory_recursive(q, dir->file_name);
    sort(q);
    format(q);
    for (uint16_t index = 0; index < q->count; index++){
        if (q->q[index]->type == directory && forbidden_hidden(q->q[index]->file_name) == false)
            traversal(q->q[index], q->path);
    }
    clean_dynamic_queue(&q);
}
