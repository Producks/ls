#include "ls.h"

static void (*print)(const struct queue *q);
static bool first = true;
static bool header = false;

#ifdef __linux__
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
    uint8_t max_link = 0;
    uint8_t max_size = 0;
    for (uint16_t i = 0; i < q->count; i++){
        total += q->q[i]->file_stat.st_blocks;
        uint8_t tmp = num_digits(q->q[i]->file_stat.st_nlink);
        if (tmp > max_link)
            max_link = tmp;
        tmp = num_digits(q->q[i]->file_stat.st_size);
        if (tmp > max_size)
            max_size = tmp;
    }
    printf("total %d\n", total / 2); // MAC WTF?
    char permission[11];
    char date[50];
    for (uint16_t i = 0; i < q->count; i++){
        permission[0] = get_type(q->q[i]->type);
        get_permission(q->q[i]->file_stat.st_mode, permission);
        format_date(&q->q[i]->file_stat.st_mtime, date);
        printf("%s %*ld %s %s %*ld %.12s %s\n", permission, max_link, q->q[i]->file_stat.st_nlink, get_owner(q->q[i]->file_stat.st_uid), get_group(q->q[i]->file_stat.st_gid), max_size, q->q[i]->file_stat.st_size, ctime(&q->q[i]->file_stat.st_mtime) + 4, q->q[i]->file_name);
    }
}
#endif

# ifdef __APPLE__

static inline void print_header(const struct queue *q)
{
    if (header == false)
        return;
    if (first == true)
        first = false;
    else
        printf("\n"); // Should be optimize to puts with O2 (hopefully)
    printf("%s:\n", q->parent_name);
}

static void print_regular(const struct queue *q)
{
    print_header(q);
    for (uint16_t i = 0; i < q->count; i++)
        printf("%s  ", q->q[i]->file_name);
    printf("\n");
}


//https://www.baeldung.com/linux/ls-total-output
static void long_list(const struct queue *q)
{
    uint32_t total = 0;
    uint8_t max_link = 0;
    uint8_t max_size = 0;
    char permission[11];

    for (uint16_t i = 0; i < q->count; i++){
        total += q->q[i]->file_stat.st_blocks;
        uint8_t tmp = num_digits(q->q[i]->file_stat.st_nlink);
        if (tmp > max_link)
            max_link = tmp;
        tmp = num_digits(q->q[i]->file_stat.st_size);
        if (tmp > max_size)
            max_size = tmp;
    }

    print_header(q);
    printf("total %d\n", total);
    for (uint16_t i = 0; i < q->count; i++){
        permission[0] = get_type(q->q[i]->type);
        get_permission(q->q[i]->file_stat.st_mode, permission);
        six_month_passed(q->q[i]->file_stat.st_mtime);
        printf("%s  %*hu %s  %s  %*lld ", permission, max_link, q->q[i]->file_stat.st_nlink, get_owner(q->q[i]->file_stat.st_uid), get_group(q->q[i]->file_stat.st_gid), max_size, q->q[i]->file_stat.st_size); // valgrind goes crazy on mac :) terrible os :)
        if (six_month_passed(q->q[i]->file_stat.st_mtime)){
            char *time_str = ctime(&q->q[i]->file_stat.st_mtime);
            remove_new_line(time_str); // Need to remove '\n; since ctime return one...
            printf("%.6s %s ", time_str + 4, time_str + 20);
        }
        else
            printf("%.12s ", ctime(&q->q[i]->file_stat.st_mtime) + 4);
        if (q->q[i]->type >= symbolic){
            char link_buffer[515];
            get_link(q->q[i]->file_name, link_buffer);
            printf("%s\n", link_buffer);
        }
        else
            printf("%s\n", q->q[i]->file_name);
    }
}
# endif

//https://www.gnu.org/software/coreutils/manual/html_node/What-information-is-listed.html

void format(const struct queue *q)
{
    if (q->count > 0)
        print(q);
}

void set_print_func(const struct ls_params *params, const struct queue *d_q, const struct queue *f_q)
{
    if (d_q->count + f_q->count > 1)
        header = true;
    if (params->long_listing == true)
        print = long_list;
    else
        print = print_regular;
}
