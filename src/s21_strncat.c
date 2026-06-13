#include "s21_string.h"

char *s21_strncat(char *dest, const char *src, s21_size_t n) {
  s21_size_t i = 0;
  s21_size_t dest_len = s21_strlen(dest);

  for (; i < n && src[i] != '\0'; i++) {
    dest[dest_len + i] = src[i];
  }

  dest[dest_len + i] = '\0';

  return dest;
}