#ifndef STDLIB_H__
#define STDLIB_H__

#include <stdbool.h>
#include <stdint.h>

#include "procyon.h"

void swap(char *a, char *b);
void reverse(char *str, int length);

int atoi(char *str);
char *itoa(int num, char *str, int base);

void *malloc(int size);

#endif
