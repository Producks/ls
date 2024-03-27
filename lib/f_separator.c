#include "ls_utils.h"

char *f_separator(const char *l, const char *r, const char c)
{
    char *result = malloc(sizeof(char) * (f_strlen(l) + f_strlen(r) + 2));
    if (!result)
        return NULL;
    size_t index = 0;
    while (*l)
        result[index++] = *l++;
    result[index++] = c;
    while (*r)
        result[index++] = *r++;
    result[index] = '\0';
    return result;
}
