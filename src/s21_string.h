#ifndef S21_STRING_H
#define S21_STRING_H
#endif  // S21_STRING_H

#include <math.h>
#include <stdarg.h>
#include <stdlib.h>

#ifndef S21_NULL
#define S21_NULL ((void *)0)
#endif  // S21_NULL
#ifndef EPSILON
#define EPSILON 1e-6
#endif  // EPSILON

typedef unsigned long s21_size_t;

void *s21_memchr(const void *str, int c, s21_size_t n);
int s21_memcmp(const void *str1, const void *str2, s21_size_t n);
void *s21_memcpy(void *dest, const void *src, s21_size_t n);
void *s21_memset(void *str, int c, s21_size_t n);
char *s21_strncat(char *dest, const char *src, s21_size_t n);
char *s21_strchr(const char *str, int c);
int s21_strncmp(const char *str1, const char *str2, s21_size_t n);
char *s21_strncpy(char *dest, const char *src, s21_size_t n);
s21_size_t s21_strcspn(const char *str1, const char *str2);
char *s21_strerror(int errnum);
s21_size_t s21_strlen(const char *str);
char *s21_strpbrk(const char *str1, const char *str2);
char *s21_strrchr(const char *str, int c);
char *s21_strstr(const char *haystack, const char *needle);
char *s21_strtok(char *str, const char *delim);
int s21_sprintf(char *str, const char *format, ...);
void *s21_to_upper(const char *str);
void *s21_to_lower(const char *str);
void *s21_insert(const char *src, const char *str, s21_size_t start_index);
void *s21_trim(const char *src, const char *trim_chars);

// for sprinf
#define ALLOWED "+- .0123456789peEcdlhfsuoxXgG"
#define BUFF_SIZE 10000
typedef struct Parameters {
  int flag_plus;
  int flag_minus;
  int flag_space;
  int flag_sharp;
  int flag_zero;
  int width;
  int precision;
  int length;
  int specifier;
  int point;
  int is_negative;
  int is_null;
} Parameters;

const char *parse_flags(const char *format, Parameters *parameters);
const char *parse_width_and_precision(const char *format,
                                      Parameters *parameters, va_list arg);
const char *parse_number(int *number, const char *format);
const char *parse_others(const char *format, Parameters *parameters,
                         char *formatted_str);
const char *parse_spicifiers(const char *format, Parameters *parameters,
                             va_list arg, char *formatted_str);
const char *main_parser(const char *format, Parameters *parameters, va_list arg,
                        char *formatted_str);
void string_specifier_handler(Parameters parameters, va_list arg,
                              char *formatted_str);
void int_specifier_handler(Parameters *parameters, va_list arg,
                           char *formatted_str);
void int_to_str(unsigned long num, char *formatted_str, unsigned long divider);
void reverse_str(char *str);
void formatting_handler(char *formatted_str, Parameters parameters);
void filling(char *formatted_str, Parameters parameters, s21_size_t len);
void add_sign(char *formatted_str, Parameters parameters);
void double_specifier_handler(Parameters *parameters, va_list arg,
                              char *formatted_str);
void double_to_string(long double num, char *formatted_str,
                      Parameters parameters);
s21_size_t get_divider(Parameters parameters);
void add_prefix(char *formatted_str, Parameters parameters);
void to_hex_str(s21_size_t num, char *formatted_str, Parameters parameters);
long double e_prepearing(long double num, int *counter);
void e_postfix(char *formatted_str, int counter, Parameters parameters);
void g_handler(Parameters *parameters, va_list arg, char *formatted_str);
void del_right_zeroes(char *str, Parameters parameters);
char get_symb_to_fill(Parameters parameters);
void add_symbols_before(char *formatted_str, char symb_to_fill, int shift);
