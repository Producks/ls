#include "ls_utils.h"

char *f_strncpy(char *dest, const char *src, size_t n)
{
    for (size_t index = 0; index < n; index++)
        dest[index] = src[index];
    return dest;
}
