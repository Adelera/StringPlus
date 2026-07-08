void *s21_to_lower(const char *str) {
  if (!str)
    return S21_NULL;
  char *result = malloc(s21_strlen(str) + 1);
  if (!result)
    return S21_NULL;
  for (s21_size_t i = 0; str[i]; i++) {
    if (str[i] >= 'A' && str[i] <= 'Z') {
      result[i] = str[i] - 'A' + 'a';
    } else {
      result[i] = str[i];
    }
  }
  result[s21_strlen(str)] = '\0';
  return result;
}