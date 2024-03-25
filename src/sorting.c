#include "ls.h"

static int8_t (*cmp)(const struct file_info *s1, const struct file_info *s2);

static int8_t cmp_ascii(const struct file_info *s1, const struct file_info *s2)
{
    return f_strcmp(s1->file_name, s2->file_name);
}

static int8_t cmp_reverse_ascii(const struct file_info *s1, const struct file_info *s2)
{
    return f_strcmp(s1->file_name, s2->file_name) > 0 ? -1 : 1;
}

static int8_t cmp_time(const struct file_info *s1, const struct file_info *s2)
{
    (void)s1;
    (void)s2;
    return 0;
}

static int8_t cmp_rev_time(const struct file_info *s1, const struct file_info *s2)
{
    (void)s1;
    (void)s2;
    return 0;
}

static void swap(struct queue *q, const int i, const int j)
{
    struct file_info *temp = q->q[i];
    q->q[i] = q->q[j];
    q->q[j] = temp;
}

void bubble_sort_LOL(struct queue *q)
{
    for (int i = 0; i < q->count; i++){
        bool s = false;
        for (int j = i + 1; j < q->count; j++){
            if (cmp(q->q[i], q->q[j]) == 1){
                swap(q, i, j);
                s = true;
            }
        }
        if (s == false)
            break;
    }
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
