#include "stdio.h"
#include "print.h"

int x = 0;
int y = 0;

int main()
{
  x = x + 2;
  y = y - 2;
  printf("here is a string passed into OSFunc $A002 in A0.L\n");

  PrintStringToDevice(0, "test\n", 0L);

  return 0;
}
