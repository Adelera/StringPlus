#include "s21_string.h"

int s21_strncmp(const char *str1, const char *str2, s21_size_t n) {
  if (n == 0) return 0;
  while (n-- && *str1 && *str2 && *str1 == *str2) {
    str1++;
    str2++;
  }
  if (n == 0 && *str1 == *str2) return 0;
  return *(unsigned char *)str1 - *(unsigned char *)str2;
}
