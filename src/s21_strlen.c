#include "s21_string.h"

s21_size_t s21_strlen(const char *str) {
  s21_size_t n = 0;
  for (; *str != '\0'; n++, str++);
  return n;
}
