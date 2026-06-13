#include "s21_string.h"

static char *s21_token_pos = S21_NULL;

// Временная функция поиска символа в строке
static int is_delim(char c, const char *delim) {
  while (*delim) {
    if (c == *delim)
      return 1;
    delim++;
  }
  return 0;
}

char *s21_strtok(char *str, const char *delim) {
  char *token_start;

  if (str != S21_NULL) {
    s21_token_pos = str;
  }

  if (s21_token_pos == S21_NULL || *s21_token_pos == '\0') {
    return S21_NULL;
  }

  // Пропускаем разделители в начале (замена s21_strchr)
  while (*s21_token_pos && is_delim(*s21_token_pos, delim)) {
    s21_token_pos++;
  }

  if (*s21_token_pos == '\0') {
    return S21_NULL;
  }

  token_start = s21_token_pos;

  // Ищем конец токена (замена s21_strchr)
  while (*s21_token_pos && !is_delim(*s21_token_pos, delim)) {
    s21_token_pos++;
  }

  if (*s21_token_pos) {
    *s21_token_pos = '\0';
    s21_token_pos++;
  }

  return token_start;
}
