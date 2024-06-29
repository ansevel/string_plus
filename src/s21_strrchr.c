#include "s21_string.h"

char *s21_strrchr(const char *str, int c) {
  const char *ptr = str + s21_strlen(str);
  for (; ptr != str && *ptr != c; ptr--);
  return *ptr == c ? (char *)ptr : S21_NULL;
}