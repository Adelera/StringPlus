#include <check.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "s21_string.h"

// ============ ТЕСТЫ ДЛЯ MEMCHR ============

START_TEST(test_memchr_found) {
  char str[] = "Hello, world!";
  ck_assert_ptr_eq(s21_memchr(str, 'o', 13), memchr(str, 'o', 13));
}
END_TEST

START_TEST(test_memchr_not_found) {
  char str[] = "Hello, world!";
  ck_assert_ptr_eq(s21_memchr(str, 'z', 13), memchr(str, 'z', 13));
}
END_TEST

START_TEST(test_memchr_zero_n) {
  char str[] = "Hello";
  ck_assert_ptr_eq(s21_memchr(str, 'H', 0), memchr(str, 'H', 0));
}
END_TEST

START_TEST(test_memchr_boundary) {
  char str[] = "Hello";
  ck_assert_ptr_eq(s21_memchr(str, 'o', 5), memchr(str, 'o', 5));
}
END_TEST

START_TEST(test_memchr_null_char) {
  char str[] = "abc";
  ck_assert_ptr_eq(s21_memchr(str, '\0', 4), memchr(str, '\0', 4));
}
END_TEST

// ============ ТЕСТЫ ДЛЯ MEMCMP ============

START_TEST(test_memcmp_equal) {
  char str1[] = "Hello";
  char str2[] = "Hello";
  ck_assert_int_eq(s21_memcmp(str1, str2, 5), memcmp(str1, str2, 5));
}
END_TEST

START_TEST(test_memcmp_first_less) {
  char str1[] = "Hello";
  char str2[] = "Helmo";
  int orig = memcmp(str1, str2, 5);
  int mine = s21_memcmp(str1, str2, 5);
  ck_assert((orig < 0 && mine < 0) || (orig > 0 && mine > 0));
}
END_TEST

START_TEST(test_memcmp_zero_n) {
  char str1[] = "Hello";
  char str2[] = "World";
  ck_assert_int_eq(s21_memcmp(str1, str2, 0), memcmp(str1, str2, 0));
}
END_TEST

START_TEST(test_memcmp_binary_data) {
  unsigned char a[] = {0, 1, 2, 200, 255};
  unsigned char b[] = {0, 1, 2, 199, 255};
  int orig = memcmp(a, b, 5);
  int mine = s21_memcmp(a, b, 5);
  ck_assert((orig < 0 && mine < 0) || (orig > 0 && mine > 0));
}
END_TEST

// ============ ТЕСТЫ ДЛЯ MEMCPY ============

START_TEST(test_memcpy_basic) {
  char src[] = "Hello";
  char dest1[10] = {0};
  char dest2[10] = {0};
  s21_memcpy(dest1, src, 5);
  memcpy(dest2, src, 5);
  ck_assert_mem_eq(dest1, dest2, 5);
}
END_TEST

START_TEST(test_memcpy_zero_n) {
  char src[] = "Hello";
  char dest1[10] = "World";
  char dest2[10] = "World";
  s21_memcpy(dest1, src, 0);
  memcpy(dest2, src, 0);
  ck_assert_mem_eq(dest1, dest2, 5);
}
END_TEST

START_TEST(test_memcpy_binary) {
  unsigned char src[] = {0, 1, 2, 3, 255, 128};
  unsigned char d1[6] = {0};
  unsigned char d2[6] = {0};
  s21_memcpy(d1, src, 6);
  memcpy(d2, src, 6);
  ck_assert_mem_eq(d1, d2, 6);
}
END_TEST

// ============ ТЕСТЫ ДЛЯ MEMSET ============

START_TEST(test_memset_basic) {
  char str1[10] = "Hello";
  char str2[10] = "Hello";
  s21_memset(str1, 'A', 3);
  memset(str2, 'A', 3);
  ck_assert_mem_eq(str1, str2, 5);
}
END_TEST

START_TEST(test_memset_zero_n) {
  char str1[10] = "Hello";
  char str2[10] = "Hello";
  volatile s21_size_t n = 0;
  s21_memset(str1, 'X', n);
  memset(str2, 'X', n);
  ck_assert_mem_eq(str1, str2, 6);
}
END_TEST

START_TEST(test_memset_full) {
  char str1[8] = {0};
  char str2[8] = {0};
  s21_memset(str1, 'Z', 7);
  memset(str2, 'Z', 7);
  ck_assert_mem_eq(str1, str2, 7);
}
END_TEST

// ============ ТЕСТЫ ДЛЯ STRLEN ============

START_TEST(test_strlen_basic) {
  ck_assert_int_eq(s21_strlen("Hello"), strlen("Hello"));
}
END_TEST

START_TEST(test_strlen_empty) { ck_assert_int_eq(s21_strlen(""), strlen("")); }
END_TEST

START_TEST(test_strlen_spaces) {
  ck_assert_int_eq(s21_strlen("  abc def  "), strlen("  abc def  "));
}
END_TEST

// ============ ТЕСТЫ ДЛЯ STRCHR ============

START_TEST(test_strchr_found) {
  char str[] = "Hello";
  ck_assert_ptr_eq(s21_strchr(str, 'l'), strchr(str, 'l'));
}
END_TEST

START_TEST(test_strchr_not_found) {
  char str[] = "Hello";
  ck_assert_ptr_eq(s21_strchr(str, 'z'), strchr(str, 'z'));
}
END_TEST

START_TEST(test_strchr_null_terminator) {
  char str[] = "Hello";
  ck_assert_ptr_eq(s21_strchr(str, '\0'), strchr(str, '\0'));
}
END_TEST

START_TEST(test_strchr_first_char) {
  char str[] = "abcabc";
  ck_assert_ptr_eq(s21_strchr(str, 'a'), strchr(str, 'a'));
}
END_TEST

// ============ ТЕСТЫ ДЛЯ STRNCAT ============

START_TEST(test_strncat_partial) {
  char d1[32] = "Hello";
  char d2[32] = "Hello";
  char *r1 = s21_strncat(d1, "World", 3);
  char *r2 = strncat(d2, "World", 3);
  ck_assert_ptr_eq(r1, d1);
  ck_assert_ptr_eq(r2, d2);
  ck_assert_str_eq(d1, d2);
}
END_TEST

START_TEST(test_strncat_full_src) {
  char d1[32] = "A";
  char d2[32] = "A";
  s21_strncat(d1, "BC", 10);
  strncat(d2, "BC", 10);
  ck_assert_str_eq(d1, d2);
}
END_TEST

START_TEST(test_strncat_zero_n) {
  char d1[32] = "prefix";
  char d2[32] = "prefix";
  s21_strncat(d1, "suffix", 0);
  strncat(d2, "suffix", 0);
  ck_assert_str_eq(d1, d2);
}
END_TEST

// ============ ТЕСТЫ ДЛЯ STRNCMP ============

START_TEST(test_strncmp_equal_prefix) {
  ck_assert_int_eq(s21_strncmp("abcdef", "abcxyz", 3),
                   strncmp("abcdef", "abcxyz", 3));
}
END_TEST

START_TEST(test_strncmp_diff_before_n) {
  int orig = strncmp("abz", "aba", 3);
  int mine = s21_strncmp("abz", "aba", 3);
  ck_assert((orig < 0 && mine < 0) || (orig > 0 && mine > 0));
}
END_TEST

START_TEST(test_strncmp_diff_after_n) {
  ck_assert_int_eq(s21_strncmp("abcX", "abcY", 3), strncmp("abcX", "abcY", 3));
}
END_TEST

START_TEST(test_strncmp_zero_n) {
  ck_assert_int_eq(s21_strncmp("abc", "xyz", 0), strncmp("abc", "xyz", 0));
}
END_TEST

// ============ ТЕСТЫ ДЛЯ STRNCPY ============

START_TEST(test_strncpy_truncate) {
  char d1[8] = {0};
  char d2[8] = {0};
  volatile s21_size_t n = 3;
  s21_strncpy(d1, "abcdef", n);
  strncpy(d2, "abcdef", n);
  ck_assert_mem_eq(d1, d2, 8);
}
END_TEST

START_TEST(test_strncpy_pad_with_zeros) {
  char d1[8] = {'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X'};
  char d2[8] = {'X', 'X', 'X', 'X', 'X', 'X', 'X', 'X'};
  s21_strncpy(d1, "ab", 6);
  strncpy(d2, "ab", 6);
  ck_assert_mem_eq(d1, d2, 8);
}
END_TEST

START_TEST(test_strncpy_zero_n) {
  char d1[8] = "hello";
  char d2[8] = "hello";
  volatile s21_size_t n = 0;
  s21_strncpy(d1, "abc", n);
  strncpy(d2, "abc", n);
  ck_assert_mem_eq(d1, d2, 8);
}
END_TEST

// ============ ТЕСТЫ ДЛЯ SPRINTF ============

START_TEST(test_sprintf_char) {
  char buf1[100] = {0};
  char buf2[100] = {0};
  int res1 =
      s21_sprintf(buf1, "Char: %c, Width: %5c, Left: %-5c", 'A', 'B', 'C');
  int res2 = sprintf(buf2, "Char: %c, Width: %5c, Left: %-5c", 'A', 'B', 'C');
  ck_assert_str_eq(buf1, buf2);
  ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(test_sprintf_string) {
  char buf1[100] = {0};
  char buf2[100] = {0};
  int res1 = s21_sprintf(buf1, "Str: %s | %15s | %-15s | %.4s | %10.2s",
                         "hello", "world", "left", "truncated", "mix");
  int res2 = sprintf(buf2, "Str: %s | %15s | %-15s | %.4s | %10.2s", "hello",
                     "world", "left", "truncated", "mix");
  ck_assert_str_eq(buf1, buf2);
  ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(test_sprintf_int_basic) {
  char buf1[100] = {0};
  char buf2[100] = {0};
  int res1 = s21_sprintf(buf1, "Int: %d | %i | %hd | %ld", 123, -456, (short)32,
                         987654321L);
  int res2 = sprintf(buf2, "Int: %d | %i | %hd | %ld", 123, -456, (short)32,
                     987654321L);
  ck_assert_str_eq(buf1, buf2);
  ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(test_sprintf_int_flags) {
  char buf1[100] = {0};
  char buf2[100] = {0};
  int res1 =
      s21_sprintf(buf1, "Flags: %+d | % d | %010d | %+010d", 42, 42, 42, 42);
  int res2 = sprintf(buf2, "Flags: %+d | % d | %010d | %+010d", 42, 42, 42, 42);
  ck_assert_str_eq(buf1, buf2);
  ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(test_sprintf_int_precision) {
  char buf1[100] = {0};
  char buf2[100] = {0};
  int res1 = s21_sprintf(buf1, "Prec: %.5d | %10.5d | %-10.5d", 42, 42, 42);
  int res2 = sprintf(buf2, "Prec: %.5d | %10.5d | %-10.5d", 42, 42, 42);
  ck_assert_str_eq(buf1, buf2);
  ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(test_sprintf_int_zero_case) {
  char buf1[100] = {0};
  char buf2[100] = {0};
  // Тот самый краеугольный камень: скрытие нуля при нулевой точности
  int res1 = s21_sprintf(buf1, "Zero: %.0d | %5.0d", 0, 0);
  int res2 = sprintf(buf2, "Zero: %.0d | %5.0d", 0, 0);
  ck_assert_str_eq(buf1, buf2);
  ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(test_sprintf_unsigned) {
  char buf1[100] = {0};
  char buf2[100] = {0};
  int res1 = s21_sprintf(buf1, "Unsigned: %u | %10u | %-10.5u | %hu",
                         3000000000U, 42, 42, (unsigned short)15);
  int res2 = sprintf(buf2, "Unsigned: %u | %10u | %-10.5u | %hu", 3000000000U,
                     42, 42, (unsigned short)15);
  ck_assert_str_eq(buf1, buf2);
  ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(test_sprintf_octal) {
  char buf1[200] = {0};
  char buf2[200] = {0};
  int res1 =
      s21_sprintf(buf1, "Octal: %o | %#o | %8.5o | %-#8.5o | %.0o | %#.0o", 83U,
                  83U, 83U, 83U, 0U, 0U);
  int res2 = sprintf(buf2, "Octal: %o | %#o | %8.5o | %-#8.5o | %.0o | %#.0o",
                     83U, 83U, 83U, 83U, 0U, 0U);
  ck_assert_str_eq(buf1, buf2);
  ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(test_sprintf_hex_lowercase) {
  char buf1[200] = {0};
  char buf2[200] = {0};
  int res1 = s21_sprintf(buf1, "Hex: %x | %#x | %08x | %#08x | %8.5x | %.0x",
                         48879U, 48879U, 48879U, 48879U, 48879U, 0U);
  int res2 = sprintf(buf2, "Hex: %x | %#x | %08x | %#08x | %8.5x | %.0x",
                     48879U, 48879U, 48879U, 48879U, 48879U, 0U);
  ck_assert_str_eq(buf1, buf2);
  ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(test_sprintf_hex_uppercase) {
  char buf1[200] = {0};
  char buf2[200] = {0};
  int res1 = s21_sprintf(buf1, "HEX: %X | %#X | %-#10.6X | %10.0X | %#.0X",
                         48879U, 48879U, 48879U, 0U, 0U);
  int res2 = sprintf(buf2, "HEX: %X | %#X | %-#10.6X | %10.0X | %#.0X", 48879U,
                     48879U, 48879U, 0U, 0U);
  ck_assert_str_eq(buf1, buf2);
  ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(test_sprintf_star) {
  char buf1[100] = {0};
  char buf2[100] = {0};
  // Динамическая ширина и точность через звездочки
  int res1 = s21_sprintf(buf1, "Star: %*.*d | %*s", 10, 5, 42, -10, "star");
  int res2 = sprintf(buf2, "Star: %*.*d | %*s", 10, 5, 42, -10, "star");
  ck_assert_str_eq(buf1, buf2);
  ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(test_sprintf_float_basic) {
  char buf1[200] = {0};
  char buf2[200] = {0};
  int res1 = s21_sprintf(buf1, "Float: %f | %.2f | %10.3f | %-10.0f", 3.5,
                         3.14159, 3.14159, 42.0);
  int res2 = sprintf(buf2, "Float: %f | %.2f | %10.3f | %-10.0f", 3.5, 3.14159,
                     3.14159, 42.0);
  ck_assert_str_eq(buf1, buf2);
  ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(test_sprintf_float_flags) {
  char buf1[200] = {0};
  char buf2[200] = {0};
  int res1 = s21_sprintf(buf1, "Flags: %+f | % f | %010.2f | %#.0f", 1.5, 1.5,
                         1.5, 2.0);
  int res2 =
      sprintf(buf2, "Flags: %+f | % f | %010.2f | %#.0f", 1.5, 1.5, 1.5, 2.0);
  ck_assert_str_eq(buf1, buf2);
  ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(test_sprintf_float_special_values) {
  char buf1[200] = {0};
  char buf2[200] = {0};
  int res1 = s21_sprintf(buf1, "Spec: %f | %f | %+f", NAN, INFINITY, -INFINITY);
  int res2 = sprintf(buf2, "Spec: %f | %f | %+f", NAN, INFINITY, -INFINITY);
  ck_assert_str_eq(buf1, buf2);
  ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(test_sprintf_scientific_lower) {
  char buf1[200] = {0};
  char buf2[200] = {0};
  int res1 =
      s21_sprintf(buf1, "Sci: %e | %.3e | %12.2e", 1234.567, 1234.567, 0.00123);
  int res2 =
      sprintf(buf2, "Sci: %e | %.3e | %12.2e", 1234.567, 1234.567, 0.00123);
  ck_assert_str_eq(buf1, buf2);
  ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(test_sprintf_scientific_upper) {
  char buf1[200] = {0};
  char buf2[200] = {0};
  int res1 =
      s21_sprintf(buf1, "SCI: %E | %#.0E | %-12.3E", 1234.567, 10.0, 0.00123);
  int res2 =
      sprintf(buf2, "SCI: %E | %#.0E | %-12.3E", 1234.567, 10.0, 0.00123);
  ck_assert_str_eq(buf1, buf2);
  ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(test_sprintf_general_lower) {
  char buf1[200] = {0};
  char buf2[200] = {0};
  int res1 = s21_sprintf(buf1, "Gen: %g | %.4g | %#.5g | %.0g", 1234.567,
                         1234.567, 12.34, 12.34);
  int res2 = sprintf(buf2, "Gen: %g | %.4g | %#.5g | %.0g", 1234.567, 1234.567,
                     12.34, 12.34);
  ck_assert_str_eq(buf1, buf2);
  ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(test_sprintf_general_upper) {
  char buf1[200] = {0};
  char buf2[200] = {0};
  int res1 = s21_sprintf(buf1, "GEN: %G | %.3G | %-#12.4G", 0.0001234, 1234.567,
                         12.34);
  int res2 =
      sprintf(buf2, "GEN: %G | %.3G | %-#12.4G", 0.0001234, 1234.567, 12.34);
  ck_assert_str_eq(buf1, buf2);
  ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(test_sprintf_pointer) {
  char buf1[200] = {0};
  char buf2[200] = {0};
  int x = 42;
  int res1 = s21_sprintf(buf1, "Ptr: %p | %20p | %-20p", (void *)&x, (void *)&x,
                         (void *)S21_NULL);
  int res2 = sprintf(buf2, "Ptr: %p | %20p | %-20p", (void *)&x, (void *)&x,
                     (void *)S21_NULL);
  ck_assert_str_eq(buf1, buf2);
  ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(test_sprintf_percent) {
  char buf1[100] = {0};
  char buf2[100] = {0};
  int res1 = s21_sprintf(buf1, "100%% done");
  int res2 = sprintf(buf2, "100%% done");
  ck_assert_str_eq(buf1, buf2);
  ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(test_sprintf_trailing_percent) {
  char buf[100] = {0};
  int res = s21_sprintf(buf, "%");
  ck_assert_str_eq(buf, "%");
  ck_assert_int_eq(res, 1);
}
END_TEST

START_TEST(test_sprintf_int_long_min) {
  char buf1[200] = {0};
  char buf2[200] = {0};
  int res1 = s21_sprintf(buf1, "%ld", LONG_MIN);
  int res2 = sprintf(buf2, "%ld", LONG_MIN);
  ck_assert_str_eq(buf1, buf2);
  ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(test_sprintf_ignore_L_for_int) {
  char buf[100] = {0};
  int res = s21_sprintf(buf, "%Ld %d", 12, 34);
  ck_assert_str_eq(buf, "12 34");
  ck_assert_int_eq(res, 5);
}
END_TEST

// ============ ТЕСТЫ ФУНКЦИЙ ПОЛИНЫ============

START_TEST(test_strcspn_basic) {
  ck_assert_int_eq(s21_strcspn("abcde", "dx"), strcspn("abcde", "dx"));
}
END_TEST

START_TEST(test_strcspn_no_match) {
  ck_assert_int_eq(s21_strcspn("hello", "XYZ"), strcspn("hello", "XYZ"));
}
END_TEST

START_TEST(test_strerror_known) {
  ck_assert_str_eq(s21_strerror(2), strerror(2));
}
END_TEST

START_TEST(test_strerror_unknown) {
  ck_assert_str_eq(s21_strerror(-1), "Unknown error -1");
}
END_TEST

START_TEST(test_strpbrk_found) {
  const char *s = "abcdef";
  ck_assert_ptr_eq(s21_strpbrk(s, "xdy"), strpbrk(s, "xdy"));
}
END_TEST

START_TEST(test_strpbrk_not_found) {
  ck_assert_ptr_eq(s21_strpbrk("abcdef", "XYZ"), S21_NULL);
}
END_TEST

START_TEST(test_strrchr_found) {
  const char *s = "abca";
  ck_assert_ptr_eq(s21_strrchr(s, 'a'), strrchr(s, 'a'));
}
END_TEST

START_TEST(test_strrchr_null_terminator) {
  const char *s = "abc";
  ck_assert_ptr_eq(s21_strrchr(s, '\0'), strrchr(s, '\0'));
}
END_TEST

START_TEST(test_strstr_found) {
  const char *s = "hello world";
  ck_assert_ptr_eq(s21_strstr(s, "world"), strstr(s, "world"));
}
END_TEST

START_TEST(test_strstr_empty_needle) {
  const char *s = "hello";
  ck_assert_ptr_eq(s21_strstr(s, ""), strstr(s, ""));
}
END_TEST

START_TEST(test_strtok_basic) {
  char src1[] = "a,b,,c";
  ck_assert_str_eq(s21_strtok(src1, ","), "a");
  ck_assert_str_eq(s21_strtok(S21_NULL, ","), "b");
  ck_assert_str_eq(s21_strtok(S21_NULL, ","), "c");
  ck_assert_ptr_eq(s21_strtok(S21_NULL, ","), S21_NULL);
}
END_TEST

START_TEST(test_s21_sscanf_basic) {
  int a1 = 0, b1 = 0;
  int a2 = 0, b2 = 0;
  int r1 = s21_sscanf("12 34", "%d %d", &a1, &b1);
  int r2 = sscanf("12 34", "%d %d", &a2, &b2);
  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(a1, a2);
  ck_assert_int_eq(b1, b2);
}
END_TEST

START_TEST(test_s21_sscanf_float_and_word) {
  float f1 = 0.0f, f2 = 0.0f;
  char s1[32] = {0}, s2[32] = {0};
  int r1 = s21_sscanf("3.14 test", "%f %s", &f1, s1);
  int r2 = sscanf("3.14 test", "%f %s", &f2, s2);
  ck_assert_int_eq(r1, r2);
  ck_assert_float_eq_tol(f1, f2, 1e-6);
  ck_assert_str_eq(s1, s2);
}
END_TEST

START_TEST(test_s21_sscanf_i_decimal_octal_hex) {
  int d1 = 0, o1 = 0, h1 = 0;
  int d2 = 0, o2 = 0, h2 = 0;
  int r1 = s21_sscanf("42 077 0x10", "%i %i %i", &d1, &o1, &h1);
  int r2 = sscanf("42 077 0x10", "%i %i %i", &d2, &o2, &h2);
  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(d1, d2);
  ck_assert_int_eq(o1, o2);
  ck_assert_int_eq(h1, h2);
}
END_TEST

START_TEST(test_s21_sscanf_unsigned_octal_hex) {
  unsigned int u1 = 0, o1 = 0, x1 = 0;
  unsigned int u2 = 0, o2 = 0, x2 = 0;
  int r1 = s21_sscanf("15 17 10", "%u %o %x", &u1, &o1, &x1);
  int r2 = sscanf("15 17 10", "%u %o %x", &u2, &o2, &x2);
  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq((int)u1, (int)u2);
  ck_assert_int_eq((int)o1, (int)o2);
  ck_assert_int_eq((int)x1, (int)x2);
}
END_TEST

START_TEST(test_s21_sscanf_length_modifiers_int) {
  short s1 = 0, s2 = 0;
  long l1 = 0, l2 = 0;
  int r1 = s21_sscanf("-12 123456", "%hd %ld", &s1, &l1);
  int r2 = sscanf("-12 123456", "%hd %ld", &s2, &l2);
  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq((int)s1, (int)s2);
  ck_assert_int_eq((int)l1, (int)l2);
}
END_TEST

START_TEST(test_s21_sscanf_length_modifiers_unsigned) {
  unsigned short s1 = 0, s2 = 0;
  unsigned long l1 = 0, l2 = 0;
  int r1 = s21_sscanf("12 123456", "%hu %lu", &s1, &l1);
  int r2 = sscanf("12 123456", "%hu %lu", &s2, &l2);
  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq((int)s1, (int)s2);
  ck_assert_int_eq((int)l1, (int)l2);
}
END_TEST

START_TEST(test_s21_sscanf_float_lengths) {
  long double ld1 = 0.0L, ld2 = 0.0L;
  double d1 = 0.0, d2 = 0.0;
  float f1 = 0.0f, f2 = 0.0f;
  int r1 = s21_sscanf("2.5e2 3.25 4.75", "%Le %lf %f", &ld1, &d1, &f1);
  int r2 = sscanf("2.5e2 3.25 4.75", "%Le %lf %f", &ld2, &d2, &f2);
  ck_assert_int_eq(r1, r2);
  ck_assert(fabsl(ld1 - ld2) < 1e-12L);
  ck_assert_double_eq_tol(d1, d2, 1e-12);
  ck_assert_float_eq_tol(f1, f2, 1e-6);
}
END_TEST

START_TEST(test_s21_sscanf_width_and_suppress) {
  char s1[16] = {0}, s2[16] = {0};
  int r1 = s21_sscanf("123 abc", "%*d %2s", s1);
  int r2 = sscanf("123 abc", "%*d %2s", s2);
  ck_assert_int_eq(r1, r2);
  ck_assert_str_eq(s1, s2);
}
END_TEST

START_TEST(test_s21_sscanf_width_for_char) {
  char c1[3] = {0}, c2[3] = {0};
  int r1 = s21_sscanf("XYZ", "%2c", c1);
  int r2 = sscanf("XYZ", "%2c", c2);
  ck_assert_int_eq(r1, r2);
  ck_assert_mem_eq(c1, c2, 2);
}
END_TEST

START_TEST(test_s21_sscanf_pointer) {
  void *p = S21_NULL;
  int r = s21_sscanf("10", "%p", &p);
  ck_assert_int_eq(r, 1);
  ck_assert_ptr_eq(p, (void *)16);
}
END_TEST

START_TEST(test_s21_sscanf_percent_literal) {
  int x1 = 0, x2 = 0;
  int r1 = s21_sscanf("25%", "%d%%", &x1);
  int r2 = sscanf("25%", "%d%%", &x2);
  ck_assert_int_eq(r1, r2);
  ck_assert_int_eq(x1, x2);
}
END_TEST

START_TEST(test_s21_sscanf_n_specifier_behavior) {
  int value = 0;
  int pos = 0;
  int r = s21_sscanf("123 abc", "%d%n %s", &value, &pos, (char[8]){0});
  ck_assert_int_eq(value, 123);
  ck_assert_int_eq(pos, 4);
  ck_assert_int_eq(r, 3);
}
END_TEST

START_TEST(test_to_upper_basic) {
  char *res = (char *)s21_to_upper("Abc 123 z");
  ck_assert_ptr_ne(res, S21_NULL);
  ck_assert_str_eq(res, "ABC 123 Z");
  free(res);
}
END_TEST

START_TEST(test_to_upper_null) {
  ck_assert_ptr_eq(s21_to_upper(S21_NULL), S21_NULL);
}
END_TEST

START_TEST(test_to_lower_basic) {
  char *res = (char *)s21_to_lower("AbC 123 Z");
  ck_assert_ptr_ne(res, S21_NULL);
  ck_assert_str_eq(res, "abc 123 z");
  free(res);
}
END_TEST

START_TEST(test_to_lower_null) {
  ck_assert_ptr_eq(s21_to_lower(S21_NULL), S21_NULL);
}
END_TEST

START_TEST(test_insert_middle) {
  char *res = (char *)s21_insert("HelloWorld", " ", 5);
  ck_assert_ptr_ne(res, S21_NULL);
  ck_assert_str_eq(res, "Hello World");
  free(res);
}
END_TEST

START_TEST(test_insert_out_of_range) {
  ck_assert_ptr_eq(s21_insert("abc", "Z", 4), S21_NULL);
}
END_TEST

START_TEST(test_trim_default_spaces) {
  char *res = (char *)s21_trim(" \tHello\n ", S21_NULL);
  ck_assert_ptr_ne(res, S21_NULL);
  ck_assert_str_eq(res, "Hello");
  free(res);
}
END_TEST

START_TEST(test_trim_custom_chars) {
  char *res = (char *)s21_trim("xxHelloxx", "x");
  ck_assert_ptr_ne(res, S21_NULL);
  ck_assert_str_eq(res, "Hello");
  free(res);
}
END_TEST

// ============ ЗАПУСК ТЕСТОВ ============

int main(void) {
  Suite *suite = suite_create("s21_string");
  TCase *tcase_core = tcase_create("core");

  // тесты для memchr
  tcase_add_test(tcase_core, test_memchr_found);
  tcase_add_test(tcase_core, test_memchr_not_found);
  tcase_add_test(tcase_core, test_memchr_zero_n);
  tcase_add_test(tcase_core, test_memchr_boundary);
  tcase_add_test(tcase_core, test_memchr_null_char);

  // тесты для memcmp
  tcase_add_test(tcase_core, test_memcmp_equal);
  tcase_add_test(tcase_core, test_memcmp_first_less);
  tcase_add_test(tcase_core, test_memcmp_zero_n);
  tcase_add_test(tcase_core, test_memcmp_binary_data);

  // тесты для memcpy
  tcase_add_test(tcase_core, test_memcpy_basic);
  tcase_add_test(tcase_core, test_memcpy_zero_n);
  tcase_add_test(tcase_core, test_memcpy_binary);

  // тесты для memset
  tcase_add_test(tcase_core, test_memset_basic);
  tcase_add_test(tcase_core, test_memset_zero_n);
  tcase_add_test(tcase_core, test_memset_full);

  // тесты для strlen
  tcase_add_test(tcase_core, test_strlen_basic);
  tcase_add_test(tcase_core, test_strlen_empty);
  tcase_add_test(tcase_core, test_strlen_spaces);

  // тесты для strchr
  tcase_add_test(tcase_core, test_strchr_found);
  tcase_add_test(tcase_core, test_strchr_not_found);
  tcase_add_test(tcase_core, test_strchr_null_terminator);
  tcase_add_test(tcase_core, test_strchr_first_char);

  // тесты для strncat
  tcase_add_test(tcase_core, test_strncat_partial);
  tcase_add_test(tcase_core, test_strncat_full_src);
  tcase_add_test(tcase_core, test_strncat_zero_n);

  // тесты для strncmp
  tcase_add_test(tcase_core, test_strncmp_equal_prefix);
  tcase_add_test(tcase_core, test_strncmp_diff_before_n);
  tcase_add_test(tcase_core, test_strncmp_diff_after_n);
  tcase_add_test(tcase_core, test_strncmp_zero_n);

  // тесты для strncpy
  tcase_add_test(tcase_core, test_strncpy_truncate);
  tcase_add_test(tcase_core, test_strncpy_pad_with_zeros);
  tcase_add_test(tcase_core, test_strncpy_zero_n);

  tcase_add_test(tcase_core, test_sprintf_char);
  tcase_add_test(tcase_core, test_sprintf_string);
  tcase_add_test(tcase_core, test_sprintf_int_basic);
  tcase_add_test(tcase_core, test_sprintf_int_flags);
  tcase_add_test(tcase_core, test_sprintf_int_precision);
  tcase_add_test(tcase_core, test_sprintf_int_zero_case);
  tcase_add_test(tcase_core, test_sprintf_unsigned);
  tcase_add_test(tcase_core, test_sprintf_octal);
  tcase_add_test(tcase_core, test_sprintf_hex_lowercase);
  tcase_add_test(tcase_core, test_sprintf_hex_uppercase);
  tcase_add_test(tcase_core, test_sprintf_star);
  tcase_add_test(tcase_core, test_sprintf_float_basic);
  tcase_add_test(tcase_core, test_sprintf_float_flags);
  tcase_add_test(tcase_core, test_sprintf_float_special_values);
  tcase_add_test(tcase_core, test_sprintf_scientific_lower);
  tcase_add_test(tcase_core, test_sprintf_scientific_upper);
  tcase_add_test(tcase_core, test_sprintf_general_lower);
  tcase_add_test(tcase_core, test_sprintf_general_upper);
  tcase_add_test(tcase_core, test_sprintf_pointer);
  tcase_add_test(tcase_core, test_sprintf_percent);
  tcase_add_test(tcase_core, test_sprintf_trailing_percent);
  tcase_add_test(tcase_core, test_sprintf_int_long_min);
  tcase_add_test(tcase_core, test_sprintf_ignore_L_for_int);
  tcase_add_test(tcase_core, test_strcspn_basic);
  tcase_add_test(tcase_core, test_strcspn_no_match);
  tcase_add_test(tcase_core, test_strerror_known);
  tcase_add_test(tcase_core, test_strerror_unknown);
  tcase_add_test(tcase_core, test_strpbrk_found);
  tcase_add_test(tcase_core, test_strpbrk_not_found);
  tcase_add_test(tcase_core, test_strrchr_found);
  tcase_add_test(tcase_core, test_strrchr_null_terminator);
  tcase_add_test(tcase_core, test_strstr_found);
  tcase_add_test(tcase_core, test_strstr_empty_needle);
  tcase_add_test(tcase_core, test_strtok_basic);
  tcase_add_test(tcase_core, test_s21_sscanf_basic);
  tcase_add_test(tcase_core, test_s21_sscanf_float_and_word);
  tcase_add_test(tcase_core, test_s21_sscanf_i_decimal_octal_hex);
  tcase_add_test(tcase_core, test_s21_sscanf_unsigned_octal_hex);
  tcase_add_test(tcase_core, test_s21_sscanf_length_modifiers_int);
  tcase_add_test(tcase_core, test_s21_sscanf_length_modifiers_unsigned);
  tcase_add_test(tcase_core, test_s21_sscanf_float_lengths);
  tcase_add_test(tcase_core, test_s21_sscanf_width_and_suppress);
  tcase_add_test(tcase_core, test_s21_sscanf_width_for_char);
  tcase_add_test(tcase_core, test_s21_sscanf_pointer);
  tcase_add_test(tcase_core, test_s21_sscanf_percent_literal);
  tcase_add_test(tcase_core, test_s21_sscanf_n_specifier_behavior);
  tcase_add_test(tcase_core, test_to_upper_basic);
  tcase_add_test(tcase_core, test_to_upper_null);
  tcase_add_test(tcase_core, test_to_lower_basic);
  tcase_add_test(tcase_core, test_to_lower_null);
  tcase_add_test(tcase_core, test_insert_middle);
  tcase_add_test(tcase_core, test_insert_out_of_range);
  tcase_add_test(tcase_core, test_trim_default_spaces);
  tcase_add_test(tcase_core, test_trim_custom_chars);

  suite_add_tcase(suite, tcase_core);

  SRunner *runner = srunner_create(suite);
  srunner_run_all(runner, CK_NORMAL);
  int failed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return failed ? 1 : 0;
}