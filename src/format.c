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

//https://www.baeldung.com/linux/ls-total-output
static void long_list(const struct queue *q)
{
    uint32_t total = 0;
    for (uint16_t i = 0; i < q->count; i++)
        total += q->q[i]->file_stat.st_blocks;
    printf("total %d\n", total); // MAC WTF?
    char permission[11];
    char time_string[50];
    for (uint16_t i = 0; i < q->count; i++){
        permission[0] = get_type(q->q[i]->type);
        get_permission(q->q[i]->file_stat.st_mode, permission);
        printf("%s %3d %s  %s  %5lld %s %s\n", permission, q->q[i]->file_stat.st_nlink, get_owner(q->q[i]->file_stat.st_uid), get_group(q->q[i]->file_stat.st_gid), q->q[i]->file_stat.st_size, ctime(&q->q[i]->file_stat.st_mtime), q->q[i]->file_name);
    }
    // print_with_header_check(q);
}

//https://www.gnu.org/software/coreutils/manual/html_node/What-information-is-listed.html

void format(const struct queue *q)
{
    if (q->count > 0)
        print(q);
}

void set_print_func(const struct ls_params *params, const struct queue *d_q, const struct queue *f_q)
{
    if ((d_q->count == 1 && f_q->count == 0 && params->recursive == false && params->long_listing == false) || (d_q->count == 0 && params->long_listing == false))
        print = print_regular;
    else if (params->long_listing == true)
        print = long_list;
    else
        print = print_with_header_check;
}
