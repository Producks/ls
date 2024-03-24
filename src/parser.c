#include "ls.h"
#include <stdio.h>

extern struct ls_params params;

void clean_params(void)
{
    for (uint64_t i = 0; i < params.argument_count; i++)
        free(params.arguments[i]);
    if (params.arguments != NULL){
        free(params.arguments);
        params.arguments = NULL;
        params.argument_count = 0;
    }
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
    for (int index = 0; str[index]; index++){
        switch(str[index]){
            case ('a'):
                params.show_hidden = true;
                break;
            case ('l'):
                params.long_listing = true;
                break;
            case ('R'):
                params.recursive = true;
                break;
            case ('r'):
                params.reverse_sort = true;
                break;
            case ('t'):
                params.time_sort = true;
                break;
            default:
                printf("ls: invalid option -- '-%s'\n", str);
                return FATAL_ERROR;
        };
    }
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
