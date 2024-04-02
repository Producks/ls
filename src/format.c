#include "ls.h"

static void (*print)(const struct queue *q);
static void (*print_header)(const struct queue *q);
static bool first = true;
static bool recursive = false;

static void print_dir_header(const struct queue *q)
{
    if (first == true)
        first = false;
    else
        printf("\n");
    if (recursive)
        printf("%s:\n", q->path);
    else
        printf("%s:\n", q->parent_name);
}

static void print_not_first_header(const struct queue *q)
{
    (void)q;
    first = false;
    print_header = print_dir_header;
}

static void print_no_header(const struct queue *q)
{
    (void)q;
}

static void print_regular(const struct queue *q)
{
    print_header(q);
    for (uint16_t i = 0; i < q->count; i++)
        printf("%s  ", q->q[i]->file_name);
    printf("\n");
}

#ifdef __linux__
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
    printf("total %d\n", total / 2);
    for (uint16_t i = 0; i < q->count; i++){
        permission[0] = get_type(q->q[i]->type);
        get_permission(q->q[i]->file_stat.st_mode, permission);
        six_month_passed(q->q[i]->file_stat.st_mtime);
        printf("%s %*lu %s %s %*ld ", permission, max_link, q->q[i]->file_stat.st_nlink, get_owner(q->q[i]->file_stat.st_uid), get_group(q->q[i]->file_stat.st_gid), max_size, q->q[i]->file_stat.st_size);
        if (six_month_passed(q->q[i]->file_stat.st_mtime)){
            char *time_str = ctime(&q->q[i]->file_stat.st_mtime);
            time_str[f_strlen(time_str) - 1] = '\0'; // Need to remove '\n; since ctime return one...
            printf("%.6s  %s ", time_str + 4, time_str + 20);
        }
        else
            printf("%.12s ", ctime(&q->q[i]->file_stat.st_mtime) + 4);
        if (q->q[i]->type >= symbolic){
            char link_buffer[LINK_MAX_LENGTH];
            if (recursive)
                get_link(q->path, q->q[i]->file_name, link_buffer);
            else
                get_link(q->parent_name, q->q[i]->file_name, link_buffer);
            printf("%s\n", link_buffer);
        }
        else
            printf("%s\n", q->q[i]->file_name);
    }
}
#endif

# ifdef __APPLE__
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
            time_str[f_strlen(time_str) - 1] = '\0'; // Need to remove '\n; since ctime return one...
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

void set_print_func(const struct ls_params *params, const struct queue *d_q, const struct queue *f_q, const int argc)
{
    if (params->recursive == true){
        recursive = true;
        if (d_q->count == 0)
            print_header = print_no_header;
        else if (f_q->count == 0)
            print_header = print_dir_header;
        else
            print_header = print_not_first_header;
    }
    else{
        if (d_q->count == 0)
            print_header = print_no_header;
        else if (d_q->count == 1 && ((argc - 1) - d_q->count == 0))
            print_header = print_no_header;
        else if (f_q->count == 0)
            print_header = print_dir_header;
        else
            print_header = print_not_first_header;
    }
    if (params->long_listing == true)
        print = long_list;
    else
        print = print_regular;
}
