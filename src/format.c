#include "ls.h"

static void (*print)(const struct queue *q);
static bool first = true;

static void print_regular(const struct queue *q)
{
    for (uint16_t i = 0; i < q->count; i++)
        printf("%s  ", q->q[i]->file_name);
    printf("\n");
}

static void print_with_header_check(const struct queue *q)
{
    if (first == true)
        first = false;
    else
        printf("\n");
    printf("%s:\n", q->parent_name);
    for (uint16_t i = 0; i < q->count; i++)
        printf("%s  ", q->q[i]->file_name);
    printf("\n");
}

void format(const struct queue *q)
{
    if (q->count > 0)
        print(q);
}

void set_print_func(const struct ls_params *params, const struct queue *d_q, const struct queue *f_q)
{
    if ((d_q->count == 1 && f_q->count == 0 && params->recursive == false) || (d_q->count == 0))
        print = print_regular;
    else
        print = print_with_header_check;
}
