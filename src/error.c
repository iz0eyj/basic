#include <stdlib.h>
#include <stdio.h>
#include <execinfo.h>

extern uint16_t __line;

const char *last_error;

  void
error(const char *error_msg)
{
  void *array[10];
  size_t size;
  char **strings;
  size_t i;

  last_error = error_msg;

  printf("--- ERROR: %d %s\n", __line, error_msg);

  size = backtrace (array, 10);
  strings = backtrace_symbols (array, size);

  printf ("Showing %zd stack frames:\n", size);

  for (i = 0; i < size; i++)
  {
    printf ("  %s\n", strings[i]);
  }

  free (strings);

  exit(1);
}