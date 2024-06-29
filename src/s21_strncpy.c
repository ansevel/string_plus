#include "s21_string.h"

char *s21_strncpy(char *dest, const char *src, s21_size_t n) {
  char *dest_ptr = dest;
  while (n && *src != '\0') {
    *dest_ptr++ = *src++;
    n--;
  }
  while (n) {
    *dest_ptr++ = '\0';
    n--;
  }
  return dest;
}
