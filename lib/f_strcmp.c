#include "ls_utils.h"

int f_strcmp(const char *s1, const char *s2)
{
    for (; *s1; s1++, s2++){
        if (*s1 != *s2)
            return *s1 < *s2 ? -1 : 1;
    }
    if (*s2 != '\0')
        return -1;
    return 0;
}
