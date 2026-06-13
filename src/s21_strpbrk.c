#define S21_NULL ((void *)0)
typedef unsigned long s21_size_t;
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