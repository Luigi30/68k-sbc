#include "stdbool.h"

extern void serial_string_out(__reg("a0") char *str);
extern void serial_char_out(__reg("d0") char c);
extern char serial_in();
extern int serial_char_waiting();

