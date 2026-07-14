#include <stdlib.h>

#include "s21_string.h"
void *s21_trim(const char *src, const char *trim_chars) {
  if (!src) return S21_NULL;
  if (!trim_chars || !*trim_chars) {
    trim_chars = " \t\n\r\f\v";
  }

  s21_size_t start = 0;
  s21_size_t end = s21_strlen(src);

  while (start < end && s21_strchr(trim_chars, src[start])) start++;
  while (end > start && s21_strchr(trim_chars, src[end - 1])) end--;

  char *result = malloc(end - start + 1);
  if (!result) return S21_NULL;

  for (s21_size_t i = start; i < end; i++) {
    result[i - start] = src[i];
  }
  result[end - start] = '\0';
  return result;
}