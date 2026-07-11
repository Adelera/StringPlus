#include "s21_string.h"
void *s21_insert(const char *src, const char *str, s21_size_t start_index) {
  if (!src || !str) return S21_NULL;
  s21_size_t src_len = s21_strlen(src);
  s21_size_t str_len = s21_strlen(str);
  if (start_index > src_len) return S21_NULL;

  char *result = malloc(src_len + str_len + 1);
  if (!result) return S21_NULL;

  s21_size_t i = 0;
  for (; i < start_index; i++) result[i] = src[i];
  for (s21_size_t j = 0; j < str_len; j++) result[i++] = str[j];
  for (; i < src_len + str_len; i++) result[i] = src[i - str_len];
  result[src_len + str_len] = '\0';

  return result;
}