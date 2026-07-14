#include <ctype.h>
#include <stdarg.h>

#include "s21_string.h"

static void skip_spaces(const char **str) {
  while (isspace(**str)) (*str)++;
}

static int is_digit(char c) { return (c >= '0' && c <= '9'); }

static int is_hex_digit(char c) {
  return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') ||
         (c >= 'A' && c <= 'F');
}

static int is_oct_digit(char c) { return (c >= '0' && c <= '7'); }

static long read_decimal(const char **str, int width, int *sign) {
  long num = 0;
  int count = 0;
  *sign = 1;

  if (**str == '-') {
    *sign = -1;
    (*str)++;
    count++;
  } else if (**str == '+') {
    (*str)++;
    count++;
  }

  while (**str && is_digit(**str) && (width == 0 || count < width)) {
    num = num * 10 + (**str - '0');
    (*str)++;
    count++;
  }

  return num;
}

static unsigned long read_unsigned(const char **str, int width, int base) {
  unsigned long num = 0;
  int count = 0;

  while (**str &&
         (base == 10  ? is_digit(**str)
          : base == 8 ? is_oct_digit(**str)
                      : is_hex_digit(**str)) &&
         (width == 0 || count < width)) {
    num = num * base + (**str - '0');
    (*str)++;
    count++;
  }

  return num;
}

static long double read_float_integer(const char **str, int width, int *count,
                                      int *sign) {
  long double num = 0.0;
  *sign = 1;

  if (**str == '-') {
    *sign = -1;
    (*str)++;
    (*count)++;
  } else if (**str == '+') {
    (*str)++;
    (*count)++;
  }

  while (**str && is_digit(**str) && (width == 0 || *count < width)) {
    num = num * 10 + (**str - '0');
    (*str)++;
    (*count)++;
  }

  return num;
}

static long double read_float_fraction(const char **str, int width, int *count,
                                       long double num) {
  if (**str == '.' && (width == 0 || *count < width)) {
    (*str)++;
    (*count)++;
    long double fraction = 0.0;
    long double divisor = 1.0;

    while (**str && is_digit(**str) && (width == 0 || *count < width)) {
      fraction = fraction * 10 + (**str - '0');
      divisor *= 10;
      (*str)++;
      (*count)++;
    }
    num = num + fraction / divisor;
  }

  return num;
}

static long double read_float_exponent(const char **str, int width, int *count,
                                       long double num) {
  if ((**str == 'e' || **str == 'E') && (width == 0 || *count < width)) {
    (*str)++;
    (*count)++;
    int exp_sign = 1;

    if (**str == '-') {
      exp_sign = -1;
      (*str)++;
      (*count)++;
    } else if (**str == '+') {
      (*str)++;
      (*count)++;
    }

    int exp = 0;
    while (**str && is_digit(**str) && (width == 0 || *count < width)) {
      exp = exp * 10 + (**str - '0');
      (*str)++;
      (*count)++;
    }

    long double power = 1.0;
    for (int i = 0; i < exp; i++) power *= 10;
    if (exp_sign == -1) power = 1.0 / power;
    num = num * power;
  }

  return num;
}

static long double read_float(const char **str, int width) {
  int count = 0;
  int sign = 1;

  long double num = read_float_integer(str, width, &count, &sign);
  num = read_float_fraction(str, width, &count, num);
  num = read_float_exponent(str, width, &count, num);

  return num * sign;
}

static long handle_i_number(const char **str, int width, int *sign) {
  long num = 0;

  if (**str == '-') {
    *sign = -1;
    (*str)++;
  } else if (**str == '+') {
    (*str)++;
  }

  if (**str == '0') {
    if (*(*str + 1) == 'x' || *(*str + 1) == 'X') {
      (*str) += 2;
      num = read_unsigned(str, width, 16);
    } else {
      num = read_unsigned(str, width, 8);
    }
  } else {
    num = read_decimal(str, width, sign);
  }

  return num;
}

static void assign_int(va_list args, char length, long value) {
  if (length == 'h') {
    short *ptr = va_arg(args, short *);
    *ptr = (short)value;
  } else if (length == 'l') {
    long *ptr = va_arg(args, long *);
    *ptr = value;
  } else {
    int *ptr = va_arg(args, int *);
    *ptr = (int)value;
  }
}

static void assign_unsigned(va_list args, char length, unsigned long value) {
  if (length == 'h') {
    unsigned short *ptr = va_arg(args, unsigned short *);
    *ptr = (unsigned short)value;
  } else if (length == 'l') {
    unsigned long *ptr = va_arg(args, unsigned long *);
    *ptr = value;
  } else {
    unsigned int *ptr = va_arg(args, unsigned int *);
    *ptr = (unsigned int)value;
  }
}

static void assign_integer(va_list args, char length, long value,
                           int is_unsigned) {
  if (is_unsigned) {
    assign_unsigned(args, length, (unsigned long)value);
  } else {
    assign_int(args, length, value);
  }
}

static int handle_integer(va_list args, char length, long signed_value,
                          unsigned long unsigned_value, int is_unsigned,
                          int suppress) {
  int result = 0;
  if (suppress == 0) {
    if (is_unsigned) {
      assign_integer(args, length, (long)unsigned_value, 1);
    } else {
      assign_integer(args, length, signed_value, 0);
    }
    result = 1;
  }
  return result;
}

static int handle_float_spec(const char **str, va_list args, int width,
                             char length, long double num, int suppress) {
  int result = 0;
  if (suppress == 0) {
    if (length == 'L') {
      long double *ptr = va_arg(args, long double *);
      *ptr = num;
    } else if (length == 'l') {
      double *ptr = va_arg(args, double *);
      *ptr = (double)num;
    } else {
      float *ptr = va_arg(args, float *);
      *ptr = (float)num;
    }
    result = 1;
  }
  (void)str;
  (void)width;
  return result;
}

static int handle_string(const char **str, va_list args, int width,
                         int suppress) {
  int result = 0;
  if (suppress != 0) {
    int w = width;
    while (**str && !isspace(**str) && (w == 0 || --w > 0)) {
      (*str)++;
    }
    result = 0;
  } else {
    char *ptr = va_arg(args, char *);
    char *start_ptr = ptr;

    while (**str && !isspace(**str) &&
           (width == 0 || (int)(ptr - start_ptr) < width)) {
      *ptr++ = **str;
      (*str)++;
    }
    *ptr = '\0';
    result = 1;
  }
  return result;
}

static int handle_char(const char **str, va_list args, int width,
                       int suppress) {
  int result = 0;
  int w = (width == 0) ? 1 : width;

  if (suppress != 0) {
    for (int i = 0; i < w; i++) (*str)++;
    result = 0;
  } else {
    char *ptr = va_arg(args, char *);
    for (int i = 0; i < w; i++) {
      ptr[i] = **str;
      (*str)++;
    }
    result = 1;
  }
  return result;
}

static int handle_pointer(const char **str, va_list args, int width,
                          int suppress) {
  int result = 0;
  unsigned long num = read_unsigned(str, width, 16);

  if (suppress == 0) {
    void **ptr = va_arg(args, void **);
    *ptr = (void *)num;
    result = 1;
  }
  return result;
}

static int handle_n(const char **str, const char *start, va_list args,
                    int suppress) {
  int result = 0;
  int *ptr = va_arg(args, int *);
  *ptr = (int)(*str - start);
  if (suppress == 0) {
    result = 1;
  }
  return result;
}

static void parse_modifiers(const char **format, int *suppress, int *width,
                            char *length) {
  *suppress = 0;
  *width = 0;
  *length = 0;

  if (**format == '*') {
    *suppress = 1;
    (*format)++;
  }

  while (is_digit(**format)) {
    *width = *width * 10 + (**format - '0');
    (*format)++;
  }

  if (**format == 'h' || **format == 'l' || **format == 'L') {
    *length = **format;
    (*format)++;
  }
}

int s21_sscanf(const char *str, const char *format, ...) {
  va_list args;
  va_start(args, format);
  int count = 0;
  const char *start = str;

  while (*format) {
    if (*format == '%') {
      format++;

      int suppress = 0;
      int width = 0;
      char length = 0;

      parse_modifiers(&format, &suppress, &width, &length);

      char specifier = *format;
      format++;

      skip_spaces(&str);

      switch (specifier) {
        case 'd': {
          int sign = 1;
          long num = read_decimal(&str, width, &sign);
          count += handle_integer(args, length, num * sign, 0, 0, suppress);
          break;
        }
        case 'i': {
          int sign = 1;
          long num = handle_i_number(&str, width, &sign);
          count += handle_integer(args, length, num * sign, 0, 0, suppress);
          break;
        }
        case 'u': {
          unsigned long num = read_unsigned(&str, width, 10);
          count += handle_integer(args, length, 0, num, 1, suppress);
          break;
        }
        case 'o': {
          unsigned long num = read_unsigned(&str, width, 8);
          count += handle_integer(args, length, 0, num, 1, suppress);
          break;
        }
        case 'x':
        case 'X': {
          unsigned long num = read_unsigned(&str, width, 16);
          count += handle_integer(args, length, 0, num, 1, suppress);
          break;
        }
        case 'f':
        case 'e':
        case 'E':
        case 'g':
        case 'G': {
          long double num = read_float(&str, width);
          count += handle_float_spec(&str, args, width, length, num, suppress);
          break;
        }
        case 'c':
          count += handle_char(&str, args, width, suppress);
          break;
        case 's':
          count += handle_string(&str, args, width, suppress);
          break;
        case 'n':
          count += handle_n(&str, start, args, suppress);
          break;
        case 'p':
          count += handle_pointer(&str, args, width, suppress);
          break;
        case '%':
          if (*str == '%') str++;
          break;
        default:
          break;
      }

    } else if (isspace(*format)) {
      format++;
      skip_spaces(&str);

    } else {
      if (*str != *format) {
        va_end(args);
        return count;
      }
      str++;
      format++;
    }
  }

  va_end(args);
  return count;
}