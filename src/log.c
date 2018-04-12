#include "log.h"

void console_log(char *tag, char *text)
{
  printf("%s: %s\n", tag, text);
}
