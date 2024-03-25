#include "ls.h"
#include <stdio.h>

extern bool show_hidden;

void clean_params(struct ls_params *params)
{
    for (uint64_t i = 0; i < params->argument_count; i++)
        free(params->arguments[i]);
    if (params->arguments != NULL){
        free(params->arguments);
        params->arguments = NULL;
        params->argument_count = 0;
    }
}

static int8_t add_argument(struct ls_params *params, const char *str)
{
    params->arguments[params->argument_count] = f_strdup(str);
    if (!params->arguments[params->argument_count])
        return fatal_error();
    params->argument_count++;
    return 0;
}

static int8_t fill_options(struct ls_params *params, const char *str)
{
    if (!*str) // edge case handling for empty -
        return add_argument(params, "-");
    for (int index = 0; str[index]; index++){
        switch(str[index]){
            case ('a'):
                show_hidden = true;
                break;
            case ('l'):
                params->long_listing = true;
                break;
            case ('R'):
                params->recursive = true;
                break;
            case ('r'):
                params->reverse_sort = true;
                break;
            case ('t'):
                params->time_sort = true;
                break;
            default:
                printf("ls: invalid option -- '-%s'\n", str);
                return FATAL_ERROR;
        };
    }
    return 0;
}

static int8_t pre_allocate(struct ls_params *params, const int argc)
{
    params->arguments = malloc(sizeof(char*) * argc); // Pre allocate to save time :)
    if (!params->arguments)
        return fatal_error();
    return SUCCESS;
}

int8_t  parse_params(struct ls_params *params, int argc, char**argv)
{
    int ret = SUCCESS;

    if (argc == 1)
        return SUCCESS;
    if (pre_allocate(params, argc))
        return FATAL_ERROR;
    for (int index = 1; index < argc && ret != FATAL_ERROR; index++){
        if (argv[index][0] == '-')
            ret = fill_options(params, argv[index] + 1);
        else
            ret = add_argument(params, argv[index]);
    }
    if (ret != FATAL_ERROR)
        params->arguments[params->argument_count] = NULL;
    return ret;
}
