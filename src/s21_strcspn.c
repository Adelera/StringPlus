#include "s21_string.h"

s21_size_t s21_strcspn(const char *str1, const char *str2) {
  s21_size_t count = 0;
  while (str1[count]) {
    for (const char *s = str2; *s; s++) {
      if (str1[count] == *s) {
        return count;
      }
    }
    count++;
  }
  return count;
}