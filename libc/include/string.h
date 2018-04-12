#ifndef STRING_H__
#define STRING_H__

#include <stddef.h>

size_t strlen(const char *str);
char *strcpy(char *dest, const char *src);
char *strncpy(char *dest, const char *src, size_t n);
int strcmp(const char *, const char *);

void *memcpy(void *destination, const void *source, size_t num);
void *memset(void *destination, int c, size_t num);

#endif
