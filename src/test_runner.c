#include <check.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <limits.h>
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

START_TEST(test_strlen_empty) {
    ck_assert_int_eq(s21_strlen(""), strlen(""));
}
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
    ck_assert_int_eq(s21_strncmp("abcdef", "abcxyz", 3), strncmp("abcdef", "abcxyz", 3));
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
    int res1 = s21_sprintf(buf1, "Char: %c, Width: %5c, Left: %-5c", 'A', 'B', 'C');
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
    int res2 = sprintf(buf2, "Str: %s | %15s | %-15s | %.4s | %10.2s", 
                           "hello", "world", "left", "truncated", "mix");
    ck_assert_str_eq(buf1, buf2);
    ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(test_sprintf_int_basic) {
    char buf1[100] = {0};
    char buf2[100] = {0};
    int res1 = s21_sprintf(buf1, "Int: %d | %i | %hd | %ld", 123, -456, (short)32, 987654321L);
    int res2 = sprintf(buf2, "Int: %d | %i | %hd | %ld", 123, -456, (short)32, 987654321L);
    ck_assert_str_eq(buf1, buf2);
    ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(test_sprintf_int_flags) {
    char buf1[100] = {0};
    char buf2[100] = {0};
    int res1 = s21_sprintf(buf1, "Flags: %+d | % d | %010d | %+010d", 42, 42, 42, 42);
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
    int res1 = s21_sprintf(buf1, "Unsigned: %u | %10u | %-10.5u | %hu", 3000000000U, 42, 42, (unsigned short)15);
    int res2 = sprintf(buf2, "Unsigned: %u | %10u | %-10.5u | %hu", 3000000000U, 42, 42, (unsigned short)15);
    ck_assert_str_eq(buf1, buf2);
    ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(test_sprintf_octal) {
    char buf1[200] = {0};
    char buf2[200] = {0};
    int res1 = s21_sprintf(buf1, "Octal: %o | %#o | %8.5o | %-#8.5o | %.0o | %#.0o",
                           83U, 83U, 83U, 83U, 0U, 0U);
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
    int res2 = sprintf(buf2, "HEX: %X | %#X | %-#10.6X | %10.0X | %#.0X",
                       48879U, 48879U, 48879U, 0U, 0U);
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
    int res1 = s21_sprintf(buf1, "Float: %f | %.2f | %10.3f | %-10.0f", 3.5, 3.14159, 3.14159, 42.0);
    int res2 = sprintf(buf2, "Float: %f | %.2f | %10.3f | %-10.0f", 3.5, 3.14159, 3.14159, 42.0);
    ck_assert_str_eq(buf1, buf2);
    ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(test_sprintf_float_flags) {
    char buf1[200] = {0};
    char buf2[200] = {0};
    int res1 = s21_sprintf(buf1, "Flags: %+f | % f | %010.2f | %#.0f", 1.5, 1.5, 1.5, 2.0);
    int res2 = sprintf(buf2, "Flags: %+f | % f | %010.2f | %#.0f", 1.5, 1.5, 1.5, 2.0);
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
    int res1 = s21_sprintf(buf1, "Sci: %e | %.3e | %12.2e", 1234.567, 1234.567, 0.00123);
    int res2 = sprintf(buf2, "Sci: %e | %.3e | %12.2e", 1234.567, 1234.567, 0.00123);
    ck_assert_str_eq(buf1, buf2);
    ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(test_sprintf_scientific_upper) {
    char buf1[200] = {0};
    char buf2[200] = {0};
    int res1 = s21_sprintf(buf1, "SCI: %E | %#.0E | %-12.3E", 1234.567, 10.0, 0.00123);
    int res2 = sprintf(buf2, "SCI: %E | %#.0E | %-12.3E", 1234.567, 10.0, 0.00123);
    ck_assert_str_eq(buf1, buf2);
    ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(test_sprintf_general_lower) {
    char buf1[200] = {0};
    char buf2[200] = {0};
    int res1 = s21_sprintf(buf1, "Gen: %g | %.4g | %#.5g | %.0g", 1234.567, 1234.567, 12.34, 12.34);
    int res2 = sprintf(buf2, "Gen: %g | %.4g | %#.5g | %.0g", 1234.567, 1234.567, 12.34, 12.34);
    ck_assert_str_eq(buf1, buf2);
    ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(test_sprintf_general_upper) {
    char buf1[200] = {0};
    char buf2[200] = {0};
    int res1 = s21_sprintf(buf1, "GEN: %G | %.3G | %-#12.4G", 0.0001234, 1234.567, 12.34);
    int res2 = sprintf(buf2, "GEN: %G | %.3G | %-#12.4G", 0.0001234, 1234.567, 12.34);
    ck_assert_str_eq(buf1, buf2);
    ck_assert_int_eq(res1, res2);
}
END_TEST

START_TEST(test_sprintf_pointer) {
    char buf1[200] = {0};
    char buf2[200] = {0};
    int x = 42;
    int res1 = s21_sprintf(buf1, "Ptr: %p | %20p | %-20p", (void *)&x, (void *)&x, (void *)S21_NULL);
    int res2 = sprintf(buf2, "Ptr: %p | %20p | %-20p", (void *)&x, (void *)&x, (void *)S21_NULL);
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

// ============ ЗАПУСК ТЕСТОВ ============

int main(void) {
    Suite *suite = suite_create("s21_string");
    TCase *tcase_core = tcase_create("core");
    
    // Добавляем тесты для memchr
    tcase_add_test(tcase_core, test_memchr_found);
    tcase_add_test(tcase_core, test_memchr_not_found);
    tcase_add_test(tcase_core, test_memchr_zero_n);
    tcase_add_test(tcase_core, test_memchr_boundary);
    tcase_add_test(tcase_core, test_memchr_null_char);
    
    // Добавляем тесты для memcmp
    tcase_add_test(tcase_core, test_memcmp_equal);
    tcase_add_test(tcase_core, test_memcmp_first_less);
    tcase_add_test(tcase_core, test_memcmp_zero_n);
    tcase_add_test(tcase_core, test_memcmp_binary_data);
    
    // Добавляем тесты для memcpy
    tcase_add_test(tcase_core, test_memcpy_basic);
    tcase_add_test(tcase_core, test_memcpy_zero_n);
    tcase_add_test(tcase_core, test_memcpy_binary);
    
    // Добавляем тесты для memset
    tcase_add_test(tcase_core, test_memset_basic);
    tcase_add_test(tcase_core, test_memset_zero_n);
    tcase_add_test(tcase_core, test_memset_full);
    
    // Добавляем тесты для strlen
    tcase_add_test(tcase_core, test_strlen_basic);
    tcase_add_test(tcase_core, test_strlen_empty);
    tcase_add_test(tcase_core, test_strlen_spaces);
    
    // Добавляем тесты для strchr
    tcase_add_test(tcase_core, test_strchr_found);
    tcase_add_test(tcase_core, test_strchr_not_found);
    tcase_add_test(tcase_core, test_strchr_null_terminator);
    tcase_add_test(tcase_core, test_strchr_first_char);

    // Добавляем тесты для strncat
    tcase_add_test(tcase_core, test_strncat_partial);
    tcase_add_test(tcase_core, test_strncat_full_src);
    tcase_add_test(tcase_core, test_strncat_zero_n);

    // Добавляем тесты для strncmp
    tcase_add_test(tcase_core, test_strncmp_equal_prefix);
    tcase_add_test(tcase_core, test_strncmp_diff_before_n);
    tcase_add_test(tcase_core, test_strncmp_diff_after_n);
    tcase_add_test(tcase_core, test_strncmp_zero_n);

    // Добавляем тесты для strncpy
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
    
    suite_add_tcase(suite, tcase_core);
    
    SRunner *runner = srunner_create(suite);
    srunner_run_all(runner, CK_NORMAL);
    int failed = srunner_ntests_failed(runner);
    srunner_free(runner);
    
    return failed ? 1 : 0;
}