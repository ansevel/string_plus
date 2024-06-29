#include "s21_string.h"

char *s21_strchr(const char *str, int c) {
  c = (unsigned char)c;
  for (; *str != c && *str != '\0'; str++);
  return *str == c ? (char *)str : S21_NULL;
}
