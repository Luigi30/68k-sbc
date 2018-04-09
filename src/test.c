extern void serial_char_out(__reg("d0") char c);
extern void serial_string_out(__reg("a0") char *str);

void tester()
{
  strlen("test");
}
