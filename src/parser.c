#include "ls.h"
#include <stdio.h>

extern struct ls_params params;

static inline int8_t fatal_error(void)
{
    perror("ls");
    return FATAL_ERROR;
}

void clean_params(void)
{
    for (uint64_t i = 0; i < params.argument_count; i++)
        free(params.arguments[i]);
    free(params.arguments);
}

static int8_t add_argument(const char *str)
{
    params.arguments[params.argument_count] = f_strdup(str);
    if (!params.arguments[params.argument_count])
        return fatal_error();
    params.argument_count++;
    return 0;
}

static int8_t fill_options(const char *str)
{
    if (!*str) // edge case handling for empty -
        return add_argument("-");
        
    return 0;
}

static int8_t pre_allocate(const int argc)
{
    params.arguments = malloc(sizeof(char*) * argc); // Pre allocate to save time :)
    if (!params.arguments)
        return fatal_error();
    return SUCCESS;
}

int8_t  parse_params(int argc, char**argv)
{
    int ret = SUCCESS;

    if (argc == 1)
        return SUCCESS;
    if (pre_allocate(argc))
        return FATAL_ERROR;
    for (int index = 1; index < argc && ret != FATAL_ERROR; index++){
        if (argv[index][0] == '-')
            ret = fill_options(argv[index] + 1);
        else
            ret = add_argument(argv[index]);
    }
    if (ret != FATAL_ERROR)
        params.arguments[params.argument_count] = NULL;
    return ret;
}
