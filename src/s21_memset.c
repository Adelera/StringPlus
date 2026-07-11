#include "s21_string.h"

void *s21_memset(void *str, int c, s21_size_t n) {
  unsigned char *buffer = (unsigned char *)str;
  const unsigned char target = (const unsigned char)c;

  for (s21_size_t i = 0; i < n; i++) {
    buffer[i] = target;
  }

  return str;
}