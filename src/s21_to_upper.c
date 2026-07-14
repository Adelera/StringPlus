#include <stdlib.h>

#include "s21_string.h"
void *s21_to_upper(const char *str) {
  if (!str) return S21_NULL;
  char *result = malloc(s21_strlen(str) + 1);
  if (!result) return S21_NULL;
  for (s21_size_t i = 0; str[i]; i++) {
    if (str[i] >= 'a' && str[i] <= 'z') {
      result[i] = str[i] - 'a' + 'A';
    } else {
      result[i] = str[i];
    }
  }
  result[s21_strlen(str)] = '\0';
  return result;
}