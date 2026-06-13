#define S21_NULL ((void *)0)
typedef unsigned long s21_size_t;
char *s21_strrchr(const char *str, int c) {
  const char *last = S21_NULL;
  while (*str) {
    if (*str == (char)c) {
      last = str;
    }
    str++;
  }
  if (c == '\0') {
    return (char *)str;
  }
  return (char *)last;
}