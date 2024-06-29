#include "s21_string.h"

void *s21_memchr(const void *str, int c, s21_size_t n) {
  unsigned const char *sym = (unsigned const char *)str;
  c = (unsigned char)c;
  while (n > 0 && *sym != c) {
    sym++;
    n--;
  }
  return n ? (void *)sym : S21_NULL;
}
