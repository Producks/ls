#ifndef LS_UTILS_H
#define LS_UTILS_H

#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void *f_memset(void *s, int c, size_t n);
size_t f_strlen(const char *s);
void f_write_fd(const char *str, int fd);
char *f_strdup(const char *s);
char *f_strncpy(char *dest, const char *src, size_t n);

#endif