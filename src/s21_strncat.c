#include "s21_string.h"

char *s21_strncat(char *dest, const char *src, s21_size_t n) {
  char *start = dest;
  while (*dest) dest++;
  for (s21_size_t i = 0; i < n && src[i]; i++) {
    *dest++ = src[i];
  }
  *dest = '\0';
  return start;
}
