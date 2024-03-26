#include "ls.h"

static void (*print)(const struct queue *q);
static bool first = true;

static void print_regular(const struct queue *q)
{
    for (uint16_t i = 0; i < q->count; i++)
        printf("%d %s  ", q->q[i]->type, q->q[i]->file_name);
    printf("\n");
}

static void print_with_header_check(const struct queue *q)
{
    if (first == true)
        first = false;
    else
        printf("\n%s:\n", q->parent_name);
    for (uint16_t i = 0; i < q->count; i++)
        printf("%d %s  ", q->q[i]->type, q->q[i]->file_name);
    printf("\n");
}

// static void print_with_header_no_check(const struct queue *q)
// {
//     if (first == true){
//         printf("%s\n", q->parent_name);
//         first = false;
//     }
//     else
//         printf("\n%s:\n", q->parent_name);
//     for (uint16_t i = 0; i < q->count; i++)
//         printf("%d %s  ", q->q[i]->type, q->q[i]->file_name);
//     printf("\n");
// }

void format(const struct queue *q)
{
    if (q->count > 0)
        print(q);
}

void set_print_func(const struct ls_params *params, const struct queue *d_q, const struct queue *f_q)
{
    (void)params;
    (void)f_q;
    if (d_q->count == 0)
        print = print_regular;
    else
        print = print_with_header_check;
}
