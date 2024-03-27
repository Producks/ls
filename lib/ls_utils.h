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
char *f_strcpy(char *dest, const char *src);
int f_strcmp(const char *s1, const char *s2);
char *f_separator(const char *l, const char *r, const char c);

#endif