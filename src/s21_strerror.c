#include <stdio.h>

#include "s21_string.h"

// Массив сообщений об ошибках (для Linux)
const char *s21_error_list[] = {
    "Success",                            // 0
    "Operation not permitted",            // 1
    "No such file or directory",          // 2
    "No such process",                    // 3
    "Interrupted system call",            // 4
    "Input/output error",                 // 5
    "No such device or address",          // 6
    "Argument list too long",             // 7
    "Exec format error",                  // 8
    "Bad file descriptor",                // 9
    "No child processes",                 // 10
    "Resource temporarily unavailable",   // 11
    "Cannot allocate memory",             // 12
    "Permission denied",                  // 13
    "Bad address",                        // 14
    "Block device required",              // 15
    "Device or resource busy",            // 16
    "File exists",                        // 17
    "Invalid cross-device link",          // 18
    "No such device",                     // 19
    "Not a directory",                    // 20
    "Is a directory",                     // 21
    "Invalid argument",                   // 22
    "Too many open files in system",      // 23
    "Too many open files",                // 24
    "Inappropriate ioctl for device",     // 25
    "Text file busy",                     // 26
    "File too large",                     // 27
    "No space left on device",            // 28
    "Illegal seek",                       // 29
    "Read-only file system",              // 30
    "Too many links",                     // 31
    "Broken pipe",                        // 32
    "Numerical argument out of domain",   // 33
    "Numerical result out of range",      // 34
    "Resource deadlock avoided",          // 35
    "File name too long",                 // 36
    "No locks available",                 // 37
    "Function not implemented",           // 38
    "Directory not empty",                // 39
    "Too many levels of symbolic links",  // 40
};

// Временная функция sprintf (только для "Unknown error %d")
static int my_sprintf(char *str, const char *format, int num) {
  char *start = str;
  while (*format) {
    if (*format == '%' && *(format + 1) == 'd') {
      format += 2;
      if (num < 0) {
        *str++ = '-';
        num = -num;
      }
      int divisor = 1;
      int temp = num;
      while (temp >= 10) {
        temp /= 10;
        divisor *= 10;
      }
      while (divisor > 0) {
        int digit = num / divisor;
        *str++ = '0' + digit;
        num %= divisor;
        divisor /= 10;
      }
    } else {
      *str++ = *format++;
    }
  }
  *str = '\0';
  return str - start;
}

// Основная функция strerror
char *s21_strerror(int errnum) {
  static char unknown[64];

  if (errnum >= 0 &&
      errnum < (int)(sizeof(s21_error_list) / sizeof(s21_error_list[0]))) {
    return (char *)s21_error_list[errnum];
  }

  my_sprintf(unknown, "Unknown error %d", errnum);
  return unknown;
}
