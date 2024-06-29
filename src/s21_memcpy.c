#include "s21_string.h"

void *s21_memcpy(void *dest, const void *src, s21_size_t n) {
  char *dest_ptr = dest;
  const char *src_ptr = src;
  for (; n > 0; n--) {
    *dest_ptr++ = *src_ptr++;
  }
  return dest;
}
