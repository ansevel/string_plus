#include "s21_string.h"

int s21_strncmp(const char *str1, const char *str2, s21_size_t n) {
  while (n && *str1 == *str2 && *str1 != '\0' && *str2 != '\0') {
    n--;
    str1++;
    str2++;
  }
  return n ? *str1 - *str2 : 0;
}
