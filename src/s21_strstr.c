#include "s21_string.h"

char *s21_strstr(const char *haystack, const char *needle) {
  char *h_ptr = (char *)haystack;
  char *res = S21_NULL;
  char *ptr;
  if (*needle == '\0') res = (char *)haystack;
  while ((ptr = s21_strchr(h_ptr, *needle)) != S21_NULL && res == S21_NULL) {
    if (!s21_strncmp(ptr, needle, s21_strlen(needle))) {
      res = ptr;
    } else {
      h_ptr = ptr + 1;
    }
  }
  return res;
}
