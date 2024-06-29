#include "s21_string.h"

char *s21_strncat(char *dest, const char *src, s21_size_t n) {
  char *dest_ptr = dest + s21_strlen(dest);
  while (n != 0 && *src != '\0') {
    *dest_ptr++ = *src++;
    n--;
  }
  *dest_ptr = '\0';
  return dest;
}
