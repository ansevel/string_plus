#include <stdio.h>  // debug mode

#include "s21_string.h"

int s21_sprintf(char *str, const char *format, ...) {
  char *buffer = str;
  char formatted_str[BUFF_SIZE];
  int symb_count = 0;
  Parameters parameters = {0};
  va_list arg;
  va_start(arg, format);

  while (*format) {
    s21_memset(&parameters, 0, sizeof(parameters));
    s21_memset(formatted_str, 0, s21_strlen(formatted_str));
    if (*format == '%') {
      format++;
      if (*format == '%') {
        *buffer++ = *format++;
        symb_count++;
      } else {
        format = main_parser(format, &parameters, arg, formatted_str);
        formatting_handler(formatted_str, parameters);
        for (s21_size_t i = 0; i < s21_strlen(formatted_str); i++) {
          *buffer++ = formatted_str[i];
          symb_count++;
        }
      }
    } else {
      *buffer++ = *format++;
      symb_count++;
    }
  }
  *buffer = '\0';
  va_end(arg);
  return symb_count;
}

const char *parse_flags(const char *format, Parameters *parameters) {
  switch (*format) {
    case '-':
      parameters->flag_minus = 1;
      break;
    case '+':
      parameters->flag_plus = 1;
      break;
    case ' ':
      parameters->flag_space = 1;
      break;
    case '#':
      parameters->flag_sharp = 1;
      break;
    case '0':
      if (!parameters->point) parameters->flag_zero = 1;
      break;
    case '.':
      parameters->point = 1;
      break;
  }
  format++;
  return format;
}

const char *parse_number(int *number, const char *format) {
  int i = 1;
  while (*format >= 48 && *format <= 57) {
    *number *= i;
    *number += (*format - 48);
    i = 10;
    format++;
  }
  return format;
}

const char *parse_width_and_precision(const char *format,
                                      Parameters *parameters, va_list arg) {
  int number = 0;
  if (!parameters->point) {
    if (*format == '*') {
      parameters->width = va_arg(arg, int);
      format++;
    } else {
      format = parse_number(&number, format);
      parameters->width = number;
    }
  } else {
    if (*format == '*') {
      parameters->precision = va_arg(arg, int);
      format++;
    } else {
      format = parse_number(&number, format);
      parameters->precision = number;
    }
  }
  return format;
}

const char *parse_others(const char *format, Parameters *parameters,
                         char *formatted_str) {
  if (s21_strchr("hlL", *format)) {
    parameters->length = *format;
    format++;
  } else if (s21_strchr(ALLOWED, *format) == S21_NULL) {
    *formatted_str = *format;
    parameters->specifier = -1;
    format++;
  }
  return format;
}

const char *parse_spicifiers(const char *format, Parameters *parameters,
                             va_list arg, char *formatted_str) {
  switch (*format) {
    case 'c':
      parameters->specifier = 'c';
      *formatted_str = va_arg(arg, int);
      break;
    case 'd':
    case 'u':
    case 'o':
    case 'x':
    case 'X':
      parameters->specifier = *format;
      int_specifier_handler(parameters, arg, formatted_str);
      break;
    case 'p':
      parameters->specifier = 'x';
      parameters->flag_sharp = 1;
      parameters->length = 'l';
      int_specifier_handler(parameters, arg, formatted_str);
      break;
    case 'E':
    case 'e':
    case 'f':
      parameters->specifier = *format;
      double_specifier_handler(parameters, arg, formatted_str);
      break;
    case 's':
      parameters->specifier = 's';
      string_specifier_handler(*parameters, arg, formatted_str);
      break;
    case 'g':
    case 'G':
      parameters->specifier = *format;
      g_handler(parameters, arg, formatted_str);
      break;
  }
  format++;
  return format;
}

const char *main_parser(const char *format, Parameters *parameters, va_list arg,
                        char *formatted_str) {
  while (*format && parameters->specifier == 0) {
    if (s21_strchr("-+ #0.", *format)) {
      format = parse_flags(format, parameters);
    } else if (s21_strchr("0123456789*", *format)) {
      format = parse_width_and_precision(format, parameters, arg);
    } else if (s21_strchr("cdufeEsoXxpgG", *format)) {
      format = parse_spicifiers(format, parameters, arg, formatted_str);
    } else {
      format = parse_others(format, parameters, formatted_str);
    }
  }
  return format;
}

void string_specifier_handler(Parameters parameters, va_list arg,
                              char *formatted_str) {
  int length;
  char *str_in;
  str_in = va_arg(arg, char *);
  length = s21_strlen(str_in);
  if (parameters.point && parameters.precision < length)
    length = parameters.precision;
  s21_strncat(formatted_str, str_in, length);
}

void int_specifier_handler(Parameters *parameters, va_list arg,
                           char *formatted_str) {
  long num;
  if (parameters->length == 'h') {
    if (s21_strchr("uoxX", parameters->specifier)) {
      num = (unsigned short)va_arg(arg, int);
    } else {
      num = (short)va_arg(arg, int);
    }
  } else if (parameters->length == 'l') {
    if (s21_strchr("uoxX", parameters->specifier)) {
      num = (unsigned long)va_arg(arg, long);
    } else {
      num = va_arg(arg, long);
    }
  } else {
    if (s21_strchr("uoxX", parameters->specifier)) {
      num = (unsigned)va_arg(arg, int);
    } else {
      num = va_arg(arg, int);
    }
  }
  if (parameters->specifier == 'd' && num < 0) {
    num = -num;
    parameters->is_negative = 1;
  }

  if (s21_strchr("Xx", parameters->specifier)) {
    to_hex_str((s21_size_t)num, formatted_str, *parameters);
  } else {
    s21_size_t divider = get_divider(*parameters);
    int_to_str((s21_size_t)num, formatted_str, divider);
  }

  if (*formatted_str == '0') parameters->is_null = 1;
  if (parameters->point) {
    if (!parameters->precision && parameters->is_null) {
      formatted_str[0] = '\0';
    } else {
      int shift = parameters->precision - s21_strlen(formatted_str);
      if (shift > 0) add_symbols_before(formatted_str, '0', shift);
    }
  }
  if (s21_strchr("oxX", parameters->specifier) && parameters->flag_sharp)
    add_prefix(formatted_str, *parameters);
  if (parameters->specifier == 'd') add_sign(formatted_str, *parameters);
}

void int_to_str(unsigned long num, char *formatted_str, unsigned long divider) {
  char *ptr = formatted_str;
  if (num == 0) *ptr++ = '0';
  while (num != 0) {
    *ptr = '0' + num % divider;
    num /= divider;
    ptr++;
  }
  *ptr = '\0';
  reverse_str(formatted_str);
}

void reverse_str(char *str) {
  int length = s21_strlen(str);
  for (int i = 0; i < length / 2; i++) {
    char temp = str[i];
    str[i] = str[length - i - 1];
    str[length - i - 1] = temp;
  }
}

void formatting_handler(char *formatted_str, Parameters parameters) {
  s21_size_t len = s21_strlen(formatted_str);
  if ((s21_size_t)parameters.width > len) {
    filling(formatted_str, parameters, len);
  }
}

void filling(char *formatted_str, Parameters parameters, s21_size_t len) {
  char symb_to_fill;

  if (parameters.flag_minus) {
    for (int i = len; i < parameters.width; i++) {
      formatted_str[i] = ' ';
    }
    formatted_str[parameters.width] = '\0';
  } else {
    int shift = parameters.width - len;

    symb_to_fill = get_symb_to_fill(parameters);
    add_symbols_before(formatted_str, symb_to_fill, shift);
    if (symb_to_fill == '0') {
      if (s21_strchr("cs", parameters.specifier) == S21_NULL &&
          (formatted_str[shift] == '+' || formatted_str[shift] == '-')) {
        formatted_str[0] = formatted_str[shift];
        formatted_str[shift] = '0';
      } else if (s21_strchr("xX", parameters.specifier) != S21_NULL &&
                 parameters.flag_sharp) {
        formatted_str[1] = 'x';
        formatted_str[shift + 1] = '0';
      }
    }
  }
}

void add_symbols_before(char *formatted_str, char symb_to_fill, int shift) {
  s21_size_t len = s21_strlen(formatted_str);
  for (s21_size_t i = 1; i <= len; i++) {
    formatted_str[len - i + shift] = formatted_str[len - i];
  }
  for (int i = 0; i < shift; i++) {
    formatted_str[i] = symb_to_fill;
  }
  formatted_str[len + shift] = '\0';
}

char get_symb_to_fill(Parameters parameters) {
  char symb_to_fill = ' ';
  if (parameters.flag_zero &&
      ((!parameters.is_null && !parameters.point) ||
       s21_strchr("scf", parameters.specifier) != S21_NULL))
    symb_to_fill = '0';
  return symb_to_fill;
}

void add_sign(char *formatted_str, Parameters parameters) {
  char sign = '\0';
  if (parameters.is_negative) {
    sign = '-';
  } else if (parameters.flag_plus) {
    sign = '+';
  } else if (parameters.flag_space) {
    sign = ' ';
  }
  if (sign) {
    s21_size_t length = s21_strlen(formatted_str);
    for (s21_size_t i = 0; i <= length; i++) {
      formatted_str[length - i + 1] = formatted_str[length - i];
    }
    formatted_str[0] = sign;
  }
}

void double_specifier_handler(Parameters *parameters, va_list arg,
                              char *formatted_str) {
  long double num;
  int counter = 0;
  if (!parameters->point) parameters->precision = 6;
  if (parameters->length == 'L') {
    num = va_arg(arg, long double);
  } else {
    num = (long double)va_arg(arg, double);
  }

  if (num < 0) {
    num = -num;
    parameters->is_negative = 1;
  }
  if (s21_strchr("Ee", parameters->specifier))
    num = e_prepearing(num, &counter);
  double_to_string(num, formatted_str, *parameters);
  if (s21_strchr("Ee", parameters->specifier))
    e_postfix(formatted_str, counter, *parameters);
  add_sign(formatted_str, *parameters);
}

void double_to_string(long double num, char *formatted_str,
                      Parameters parameters) {
  if (!parameters.precision) {
    num = round(num);
    int_to_str((s21_size_t)num, formatted_str, 10UL);
    if (parameters.flag_sharp) formatted_str[s21_strlen(formatted_str)] = '.';
  } else {
    long double int_part = 0;
    long double frac_part = 0;
    frac_part = modfl(num, &int_part);
    s21_size_t flag_int_plus = 1;
    for (int i = parameters.precision; i > 0; i--) {
      frac_part *= 10;
      flag_int_plus *= 10;
    }
    frac_part = round(frac_part);
    flag_int_plus = frac_part >= flag_int_plus ? 1UL : 0UL;

    int_to_str((s21_size_t)int_part + flag_int_plus, formatted_str, 10UL);
    formatted_str[s21_strlen(formatted_str)] = '.';
    int length = parameters.precision + 2;
    char frac_str[length];
    s21_memset(frac_str, 0, length);
    int_to_str((s21_size_t)frac_part, frac_str, 10UL);
    if (flag_int_plus) {
      for (int i = 0; i < length - 1; i++) {
        frac_str[i] = frac_str[i + 1];
      }
    }
    s21_strncat(formatted_str, "000000000000000",
                parameters.precision - s21_strlen(frac_str));
    s21_strncat(formatted_str, frac_str, length);
  }
}

s21_size_t get_divider(Parameters parameters) {
  s21_size_t divider = 1UL;
  if (s21_strchr("du", parameters.specifier)) {
    divider = 10UL;
  } else if (parameters.specifier == 'o') {
    divider = 8UL;
  }
  return divider;
}

void add_prefix(char *formatted_str, Parameters parameters) {
  s21_size_t len = s21_strlen(formatted_str);
  if (parameters.specifier == 'o' && *formatted_str != '0') {
    for (s21_size_t i = len; i > 0; i--) {
      formatted_str[i] = formatted_str[i - 1];
    }
    formatted_str[0] = '0';
  } else if (s21_strchr("xX", parameters.specifier) != S21_NULL &&
             !parameters.is_null) {
    for (s21_size_t i = len + 1; i > 1; i--) {
      formatted_str[i] = formatted_str[i - 2];
    }
    formatted_str[0] = '0';
    formatted_str[1] = parameters.specifier;
  }
}

void to_hex_str(s21_size_t num, char *formatted_str, Parameters parameters) {
  char *ptr = formatted_str;
  int digit;
  if (num == 0) *ptr++ = '0';
  while (num != 0) {
    digit = num % 16UL;
    if (digit > 9) {
      *ptr = digit - 10 + 'A' + (parameters.specifier - 'X');
    } else {
      *ptr = digit + '0';
    }
    num /= 16UL;
    ptr++;
  }
  *ptr = '\0';
  reverse_str(formatted_str);
}

long double e_prepearing(long double num, int *counter) {
  if (num < -EPSILON || num > EPSILON) {
    if (num >= 1) {
      while (num >= 10) {
        num /= 10;
        (*counter)++;
      }
    } else {
      while (num < 1.0L) {
        num *= 10.0L;
        (*counter)--;
      }
    }
  } else {
    num = 0.0;
  }
  return num;
}

void e_postfix(char *formatted_str, int counter, Parameters parameters) {
  s21_size_t idx = s21_strlen(formatted_str);
  formatted_str[idx++] = 'E' + (parameters.specifier - 'E');
  if (counter < 0) {
    formatted_str[idx++] = '-';
    counter = -counter;
  } else {
    formatted_str[idx++] = '+';
  }
  if (counter < 10) {
    formatted_str[idx++] = '0';
    formatted_str[idx++] = counter + '0';
  } else {
    formatted_str[idx++] = (counter / 10) % 10 + '0';
    formatted_str[idx++] = counter % 10 + '0';
  }
  formatted_str[idx] = '\0';
}

void g_handler(Parameters *parameters, va_list arg, char *formatted_str) {
  long double number;
  char exp_buf[BUFF_SIZE] = {0};
  char exp_buf_with_null[BUFF_SIZE] = {0};
  char double_buf[BUFF_SIZE] = {0};
  int shift = parameters->specifier - 'G';
  if (!parameters->point) {
    parameters->precision = 6;
  } else if (!parameters->precision) {
    parameters->precision = 1;
  }

  number = va_arg(arg, double);
  if (number > -EPSILON && number < EPSILON) number = 0.0;
  if (number < 0) {
    number = -number;
    parameters->is_negative = 1;
  }
  Parameters parameters_e = *parameters;
  Parameters parameters_f = *parameters;
  parameters_e.specifier = 'E' + shift;
  parameters_f.specifier = 'f';

  int counter = 0;
  double num_e = e_prepearing(number, &counter);

  if (parameters->precision > counter) parameters_f.precision -= counter + 1;
  parameters_e.precision -= 1;
  double_to_string(number, double_buf, parameters_f);
  del_right_zeroes(double_buf, parameters_f);

  double_to_string(num_e, exp_buf, parameters_e);
  s21_strncat(exp_buf_with_null, exp_buf, s21_strlen(exp_buf));
  del_right_zeroes(exp_buf, parameters_e);
  e_postfix(exp_buf, counter, parameters_e);
  e_postfix(exp_buf_with_null, counter, parameters_e);

  s21_size_t len_e = s21_strlen(exp_buf);
  s21_size_t len_f = s21_strlen(double_buf);

  if (len_e <= len_f) {
    if (!parameters->flag_sharp) {
      s21_strncat(formatted_str, exp_buf, s21_strlen(exp_buf));
    } else {
      s21_strncat(formatted_str, exp_buf_with_null,
                  s21_strlen(exp_buf_with_null));
    }
  } else {
    s21_strncpy(formatted_str, double_buf, len_f);
  }

  add_sign(formatted_str, *parameters);
}

void del_right_zeroes(char *str, Parameters parameters) {
  s21_size_t idx;
  idx = s21_strlen(str) - 1;
  while (str[idx] == '0') {
    str[idx] = '\0';
    idx--;
  }
  if (str[idx] == '.' && !parameters.flag_sharp) str[idx] = '\0';
}
