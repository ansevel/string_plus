
#include "s21_string.h"

void *s21_to_lower(const char *str) {
  char *res = S21_NULL;
  if (str != S21_NULL) {
    s21_size_t len = s21_strlen(str);
    res = (char *)calloc(len + 1, sizeof(char));
    for (s21_size_t i = 0; str[i]; i++) {
      if (str[i] >= 'A' && str[i] <= 'Z') {
        res[i] = str[i] + ('a' - 'A');
      } else {
        res[i] = str[i];
      }
    }
  }
  return res;
}
