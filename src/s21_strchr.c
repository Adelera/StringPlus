#include "s21_string.h"

char *s21_strchr(const char *str, int c) {
  char ch = (char)c;
  s21_size_t i = 0;

  while (str[i] != '\0') {
    if (str[i] == ch) {
      return (char *)(str + i);
    }

    i++;
  }

  if (ch == '\0') {
    return (char *)(str + i);
  }

  return S21_NULL;
}