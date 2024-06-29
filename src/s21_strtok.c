#include "s21_string.h"

char *s21_strtok(char *str, const char *delim) {
  static char *last_token = S21_NULL;
  char *token = S21_NULL;
  if (str != S21_NULL) {
    last_token = str;
  }
  token = last_token;
  if (token != S21_NULL && *token == '\0') token = S21_NULL;
  while (token != S21_NULL && s21_strchr(delim, *token) != S21_NULL) {
    token++;
  }
  if (token != S21_NULL) {
    char *end_token = token;
    while (*end_token != '\0' && s21_strchr(delim, *end_token) == S21_NULL) {
      end_token++;
    }
    if (*end_token != '\0') {
      *end_token = '\0';
      end_token++;
      last_token = end_token;
    } else {
      last_token = S21_NULL;
    }
  }
  return token;
}
