
#include "s21_string.h"

void *s21_insert(const char *src, const char *str, s21_size_t start_index) {
  char *res = S21_NULL;
  if (src != S21_NULL) {
    s21_size_t len_src = s21_strlen(src);
    if (src && str && len_src >= start_index) {
      s21_size_t len_str = s21_strlen(str);
      res = calloc(len_src + len_str + 1, sizeof(char));
      if (res) {
        s21_strncpy(res, src, start_index);
        s21_strncpy(res + start_index, str, s21_strlen(str));
        s21_strncpy(res + start_index + len_str, src + start_index,
                    s21_strlen(str));
      }
    }
  }
  return res;
}