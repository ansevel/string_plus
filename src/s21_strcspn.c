#include "s21_string.h"

s21_size_t s21_strcspn(const char *str1, const char *str2) {
  s21_size_t res = 0;
  while (*str1 != '\0' && !s21_strchr(str2, *str1)) {
    res++;
    str1++;
  }
  return res;
}
