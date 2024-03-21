#include "../lib/ls_utils.h"
#include <stdio.h>

int main(void)
{
    printf("%p\n", f_memset(NULL, 0, 5));
    return 0;
}