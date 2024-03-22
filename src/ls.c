#include "ls.h"
#include <stdio.h>

struct ls_params params = {false, false, false, normal, 0, NULL};

int ls(int argc, char **argv)
{
    if (parse_params(argc, argv))
        return 1;
    for (uint64_t i = 0; i < params.argument_count; i++)
        printf("%s\n", params.arguments[i]);
    clean_params();
    return SUCCESS;
}
