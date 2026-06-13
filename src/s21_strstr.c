#define S21_NULL ((void *)0)
typedef unsigned long s21_size_t;
char *s21_strstr(const char *haystack, const char *needle) {
  if (!*needle) {
    return (char *)haystack;
  }
  for (; *haystack; haystack++) {
    const char *h = haystack;
    const char *n = needle;
    while (*h && *n && *h == *n) {
      h++;
      n++;
    }
    if (!*n) {
      return (char *)haystack;
    }
  }
  return S21_NULL;
}