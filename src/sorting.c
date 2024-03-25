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

static void swap(struct queue *q, const int i, const int j)
{
    struct file_info *temp = q->q[i];
    q->q[i] = q->q[j];
    q->q[j] = temp;
}

// -1 == j < pivot
//  1 == j > pivot
uint32_t partition(struct queue *q, int low, int high)
{
    int pivot = high;
    int i = low;
    for (int j = low; j < high; j++){
        if (cmp(q->q[j], q->q[pivot]) < 0)
            swap(q, i++, j);
    }
    swap(q, i, high);
    return i;
}

void quicksort(struct queue *q, int low, int high)
{
    if (low < high){
        int pivot_index = partition(q, low, high);
        quicksort(q, low, pivot_index - 1);
        quicksort(q, low + 1, high);
    }
}

void sort(struct queue *q)
{
    if (q->count == 0)
        return;
    quicksort(q, 0, q->count - 1);
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
