
#include "s21_string.h"

void *s21_trim(const char *src, const char *trim_chars) {
  char *res = S21_NULL;
  if (src && trim_chars) {
    s21_size_t src_len = s21_strlen(src);
    const char *res_start = src;
    const char *res_end = src + src_len - 1;

    // Trim from the start
    while (*res_start && s21_strchr(trim_chars, *res_start)) {
      res_start++;
    }

    // Trim from the end
    while (res_end >= res_start && s21_strchr(trim_chars, *res_end)) {
      res_end--;
    }

    s21_size_t new_len = res_end - res_start + 1;
    res = calloc(new_len + 1, sizeof(char));
    if (res) {
      s21_memcpy(res, res_start, new_len);
      res[new_len] = '\0';
    }
  }
  return res;
}
