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
