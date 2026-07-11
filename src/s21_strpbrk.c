#include "s21_string.h"
char *s21_strpbrk(const char *str1, const char *str2) {
  for (const char *p = str1; *p; p++) {
    for (const char *s = str2; *s; s++) {
      if (*p == *s) {
        return (char *)p;
      }
    }
  }
  return S21_NULL;
}