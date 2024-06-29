#include "s21_string.h"

void *s21_memset(void *str, int c, s21_size_t n) {
  char *str_ptr = str;
  c = (unsigned char)c;
  for (; n > 0; n--) {
    *str_ptr++ = c;
  }
  return str;
}
