#include "ls_utils.h"

void *f_memset(void *s, int c, size_t n)
{
	register unsigned char	*ptr = (unsigned char*)s;

	while (n-- > 0)
		*ptr++ = c;
	return (ptr);
}