#include "s21_string.h"

static char *s21_token_pos = S21_NULL;

char *s21_strtok(char *str, const char *delim) {
  char *token_start;

  if (str != S21_NULL) {
    s21_token_pos = str;
  }

  if (s21_token_pos == S21_NULL || *s21_token_pos == '\0') {
    return S21_NULL;
  }

  while (*s21_token_pos && s21_strchr(delim, *s21_token_pos)) {
    s21_token_pos++;
  }

  if (*s21_token_pos == '\0') {
    return S21_NULL;
  }

  token_start = s21_token_pos;

  while (*s21_token_pos && !s21_strchr(delim, *s21_token_pos)) {
    s21_token_pos++;
  }

  if (*s21_token_pos) {
    *s21_token_pos = '\0';
    s21_token_pos++;
  }

  return token_start;
}