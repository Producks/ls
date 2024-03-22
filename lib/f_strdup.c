#include "ls_utils.h"

char *f_strdup(const char *s)
{
    size_t length = f_strlen(s) + 1;
    char *result = malloc(sizeof(char) * length);
    if (!result)
        return NULL;
    return f_strncpy(result, s, length);
}
