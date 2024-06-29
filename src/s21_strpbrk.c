#include "s21_string.h"

char *s21_strpbrk(const char *str1, const char *str2) {
  const char *ptr = str1;
  for (; *ptr != '\0' && !s21_strchr(str2, *ptr); ptr++);
  return *ptr == '\0' ? S21_NULL : (char *)ptr;
}
