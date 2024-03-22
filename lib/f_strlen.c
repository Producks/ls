#include "ls_utils.h"

size_t f_strlen(const char *s)
{
    const char *c = s;
    while (*s)
        s++;
    return s - c;
}
