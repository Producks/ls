#include "ls.h"

static int8_t (*cmp)(const struct file_info *s1, const struct file_info *s2);

static int8_t cmp_ascii(const struct file_info *s1, const struct file_info *s2)
{
    return f_strcmp(s1->file_name, s2->file_name);
}

static int8_t cmp_reverse_ascii(const struct file_info *s1, const struct file_info *s2)
{
    return -f_strcmp(s1->file_name, s2->file_name);
}

static int8_t cmp_time(const struct file_info *s1, const struct file_info *s2)
{
    return s1->file_stat.st_mtime < s2->file_stat.st_mtime ? 1 : -1;
}

static int8_t cmp_rev_time(const struct file_info *s1, const struct file_info *s2)
{
    return s1->file_stat.st_mtime < s2->file_stat.st_mtime ? -1 : 1;
}

static void insertion_sort(struct queue *q)
{
    for (int i = 1; i < q->count; i++){
        struct file_info *tmp = q->q[i];
        int j = i - 1;
        while (j >= 0 && cmp(q->q[j], tmp) > 0){
            q->q[j + 1] = q->q[j];
            j--;
        }
        q->q[j + 1] = tmp;
    }
}

void sort(struct queue *q)
{
    if (q->count == 0)
        return;
    insertion_sort(q);
}

void set_cmp_func(struct ls_params *params)
{
    if (params->reverse_sort == false && params->time_sort == false)
        cmp = cmp_ascii;
    else if (params->reverse_sort == true && params->time_sort == false)
        cmp = cmp_reverse_ascii;
    else if (params->reverse_sort == false)
        cmp = cmp_time;
    else
        cmp = cmp_rev_time;
}
