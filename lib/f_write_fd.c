#include "ls_utils.h"

void f_write_fd(const char *str, int fd)
{
    write(fd, str, f_strlen(str));
}
