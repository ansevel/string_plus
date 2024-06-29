#include "s21_string.h"

int s21_memcmp(const void *str1, const void *str2, s21_size_t n) {
  const char *s1_ptr = str1;
  const char *s2_ptr = str2;
  while (*s1_ptr == *s2_ptr && n > 0) {
    n--;
    s1_ptr++;
    s2_ptr++;
  }
  return n ? *s1_ptr - *s2_ptr : 0;
}
