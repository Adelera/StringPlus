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
         (base == 10
              ? is_digit(**str)
              : (base == 8 ? is_oct_digit(**str) : is_hex_digit(**str))) &&
         (width == 0 || count < width)) {
    num = num * base + (**str - '0');
    (*str)++;
    count++;
  }

  return num;
}

static long double read_float(const char **str, int width) {
  long double num = 0.0;
  long double fraction = 0.0;
  int count = 0;
  int sign = 1;

  if (**str == '-') {
    sign = -1;
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

  if (**str == '.' && (width == 0 || count < width)) {
    (*str)++;
    count++;
    long double divisor = 1.0;
    while (**str && is_digit(**str) && (width == 0 || count < width)) {
      fraction = fraction * 10 + (**str - '0');
      divisor *= 10;
      (*str)++;
      count++;
    }
    num = num + fraction / divisor;
  }

  if ((**str == 'e' || **str == 'E') && (width == 0 || count < width)) {
    (*str)++;
    count++;
    int exp_sign = 1;
    if (**str == '-') {
      exp_sign = -1;
      (*str)++;
      count++;
    } else if (**str == '+') {
      (*str)++;
      count++;
    }

    int exp = 0;
    while (**str && is_digit(**str) && (width == 0 || count < width)) {
      exp = exp * 10 + (**str - '0');
      (*str)++;
      count++;
    }

    long double power = 1.0;
    for (int i = 0; i < exp; i++) power *= 10;
    if (exp_sign == -1) power = 1.0 / power;
    num = num * power;
  }

  return num * sign;
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
      if (*format == '*') {
        suppress = 1;
        format++;
      }

      int width = 0;
      while (is_digit(*format)) {
        width = width * 10 + (*format - '0');
        format++;
      }

      char length = 0;
      if (*format == 'h' || *format == 'l' || *format == 'L') {
        length = *format;
        format++;
      }

      char specifier = *format;
      format++;

      skip_spaces(&str);

      if (specifier == 'd' || specifier == 'i') {
        int sign = 1;
        long num = 0;

        if (specifier == 'i') {
          if (*str == '-') {
            sign = -1;
            str++;
          } else if (*str == '+') {
            str++;
          }

          if (*str == '0') {
            if (*(str + 1) == 'x' || *(str + 1) == 'X') {
              str += 2;
              num = read_unsigned(&str, width, 16);
            } else {
              num = read_unsigned(&str, width, 8);
            }
          } else {
            num = read_decimal(&str, width, &sign);
          }
        } else {
          num = read_decimal(&str, width, &sign);
        }

        if (!suppress) {
          if (length == 'h') {
            short *ptr = va_arg(args, short *);
            *ptr = (short)(num * sign);
          } else if (length == 'l') {
            long *ptr = va_arg(args, long *);
            *ptr = num * sign;
          } else {
            int *ptr = va_arg(args, int *);
            *ptr = (int)(num * sign);
          }
          count++;
        }
      } else if (specifier == 'u') {
        unsigned long num = read_unsigned(&str, width, 10);
        if (!suppress) {
          if (length == 'h') {
            unsigned short *ptr = va_arg(args, unsigned short *);
            *ptr = (unsigned short)num;
          } else if (length == 'l') {
            unsigned long *ptr = va_arg(args, unsigned long *);
            *ptr = num;
          } else {
            unsigned int *ptr = va_arg(args, unsigned int *);
            *ptr = (unsigned int)num;
          }
          count++;
        }
      } else if (specifier == 'o') {
        unsigned long num = read_unsigned(&str, width, 8);
        if (!suppress) {
          if (length == 'h') {
            unsigned short *ptr = va_arg(args, unsigned short *);
            *ptr = (unsigned short)num;
          } else if (length == 'l') {
            unsigned long *ptr = va_arg(args, unsigned long *);
            *ptr = num;
          } else {
            unsigned int *ptr = va_arg(args, unsigned int *);
            *ptr = (unsigned int)num;
          }
          count++;
        }
      } else if (specifier == 'x' || specifier == 'X') {
        unsigned long num = read_unsigned(&str, width, 16);
        if (!suppress) {
          if (length == 'h') {
            unsigned short *ptr = va_arg(args, unsigned short *);
            *ptr = (unsigned short)num;
          } else if (length == 'l') {
            unsigned long *ptr = va_arg(args, unsigned long *);
            *ptr = num;
          } else {
            unsigned int *ptr = va_arg(args, unsigned int *);
            *ptr = (unsigned int)num;
          }
          count++;
        }
      } else if (specifier == 'f' || specifier == 'e' || specifier == 'E' ||
                 specifier == 'g' || specifier == 'G') {
        long double num = read_float(&str, width);
        if (!suppress) {
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
          count++;
        }
      } else if (specifier == 'c') {
        if (width == 0) width = 1;
        if (!suppress) {
          char *ptr = va_arg(args, char *);
          for (int i = 0; i < width; i++) {
            ptr[i] = *str++;
          }
          count++;
        } else {
          for (int i = 0; i < width; i++) {
            str++;
          }
        }
      } else if (specifier == 's') {
        if (!suppress) {
          char *ptr = va_arg(args, char *);
          char *start_ptr = ptr;
          while (*str && !isspace(*str) &&
                 (width == 0 || (int)(ptr - start_ptr) < width)) {
            *ptr++ = *str++;
          }
          *ptr = '\0';
          count++;
        } else {
          int w = width;
          while (*str && !isspace(*str) && (w == 0 || --w > 0)) {
            str++;
          }
        }
      } else if (specifier == 'n') {
        int *ptr = va_arg(args, int *);
        *ptr = (int)(str - start);
        if (!suppress) count++;
      } else if (specifier == 'p') {
        unsigned long num = read_unsigned(&str, width, 16);
        if (!suppress) {
          void **ptr = va_arg(args, void **);
          *ptr = (void *)num;
          count++;
        }
      } else if (specifier == '%') {
        if (*str == '%') str++;
      }
    } else if (isspace(*format)) {
      format++;
      skip_spaces(&str);
    } else {
      if (*str != *format) break;
      str++;
      format++;
    }
  }

  va_end(args);
  return count;
}