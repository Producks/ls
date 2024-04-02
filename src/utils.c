#include "ls.h"

static enum file_type check_type(__mode_t mode)
{
    if (S_ISREG(mode))
        return file;
    else if (S_ISDIR(mode))
        return directory;
    else if (S_ISLNK(mode))
        return symbolic;
    else if (S_ISBLK(mode))
        return blk;
    else if (S_ISCHR(mode))
        return chr;
    else if (S_ISFIFO(mode))
        return fifo;
    else if (S_ISSOCK(mode))
        return socket;
    return invalid;
}

char get_type(const enum file_type type)
{
    static const char lookup_table_file[] = { '!', 'd', '-', 'p', 'b', 'c', 's', 'l', 'l', 'l', 'l', 'l', 'l', 'l', 'l'}; // added ! for padding since enum start at 1
    return lookup_table_file[type];
}

void get_permission(const __mode_t mode, char *buffer)
{
    uint8_t index = 1;
    buffer[index++] = OW_R(mode) ? 'r' : '-'; 
    buffer[index++] = OW_W(mode) ? 'w' : '-'; 
    buffer[index++] = OW_E(mode) ? 'x' : '-';

    buffer[index++] = G_R(mode) ? 'r' : '-';
    buffer[index++] = G_W(mode) ? 'w' : '-';
    buffer[index++] = G_E(mode) ? 'x' : '-';

    buffer[index++] = OT_R(mode) ? 'r' : '-';
    buffer[index++] = OT_W(mode) ? 'w' : '-';
    buffer[index++] = OT_E(mode) ? 'x' : '-';

    buffer[index] = '\0';
}

static int8_t set_type(const char *str, struct file_info *file)
{
    if (lstat(str, &file->file_stat) == FATAL_ERROR){
        (void)printf("ls: cannot open directory '%s': %s\n", str, strerror(errno));
        return FATAL_ERROR;
    }
    file->type = check_type(file->file_stat.st_mode);
    if (file->type == symbolic){
        struct stat tmp;
        if (stat(str, &tmp) == FATAL_ERROR){
            file->type = broken_sym;
            return SUCCESS;
        }
        file->type += check_type(tmp.st_mode);
    }
    return SUCCESS;
}

struct file_info *create_file_info(const char *file_path, const char *file_name)
{
    struct file_info *file = malloc(sizeof(struct file_info));
    if (!file){
        perror("ls");
        return NULL;
    }
    if (set_type(file_path, file) == FATAL_ERROR){
        free(file);
        return NULL;
    }
    f_strcpy(file->file_name, file_name);
    return file;
}

char *get_real_folder(const char *path)
{
    char buffer[PATH_MAX + 1];
    char arg[PATH_MAX + 1];
    f_strcpy(arg, path);
    int ret;
    while (true){
        ret = readlink(arg, buffer, PATH_MAX);
        if (ret != -1)
            buffer[ret] = '\0';
        else
            break;
        f_strcpy(arg, buffer);
    }
    return f_strdup(buffer);
}

bool is_hidden(const char *str)
{
    return *str == '.' ? true : false;
}

char *get_owner(uid_t id)
{
    struct passwd *result = getpwuid(id);
    if (!result)
        return NULL;
    return result->pw_name;
}

char *get_group(uid_t id)
{
    struct group *result = getgrgid(id);
    if (!result)
        return NULL;
    return result->gr_name;
}

uint8_t num_digits(uint32_t n)
{
    if (n < 10) 
        return 1;
    if (n < 100)
        return 2;
    if (n < 1000)
        return 3;
    if (n < 10000)
        return 4;
    if (n < 100000)
        return 5;
    if (n < 1000000)
        return 6;
    if (n < 10000000)
        return 7;
    if (n < 100000000)
        return 8;
    if (n < 1000000000)
        return 9;
    return 10;
}

bool six_month_passed(const time_t date)
{
    static time_t timestamp = 0;

    if (timestamp == 0)
        timestamp = time(NULL);
    if (timestamp - date > SIX_MONTH || date - timestamp < -SIX_MONTH)
        return true;
    return false;
}

// Optimize TODO
void get_link(const char *path, const char *file_name, char *buffer)
{
    uint16_t index = 0;
    for (; file_name[index]; index++)
        buffer[index] = file_name[index];
    f_strcpy(buffer + index, " -> ");
    index += 4;
    char *test = f_separator(path, file_name , '/');
    const ssize_t result = readlink(test, buffer + index, 255);
    buffer[index + result] = '\0';
    free(test);
}

bool forbidden_hidden(const char *str)
{
    if (!f_strcmp(".", str) || !f_strcmp("..", str))
        return true;
    return false;
}
