#define S21_NULL ((void *)0)
typedef unsigned long s21_size_t;
char *s21_strncpy(char *dest, const char *src, s21_size_t n) {
  char *start = dest;
  s21_size_t i;
  for (i = 0; i < n && src[i]; i++) {
    dest[i] = src[i];
  }
  for (; i < n; i++) {
    dest[i] = '\0';
  }
  return start;
}