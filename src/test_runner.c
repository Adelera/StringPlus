#include <check.h>
#include <string.h>
#include <stdlib.h>
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

// ============ ТЕСТЫ ДЛЯ MEMSET ============

START_TEST(test_memset_basic) {
    char str1[10] = "Hello";
    char str2[10] = "Hello";
    s21_memset(str1, 'A', 3);
    memset(str2, 'A', 3);
    ck_assert_mem_eq(str1, str2, 5);
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

// ============ ЗАПУСК ТЕСТОВ ============

int main(void) {
    Suite *suite = suite_create("s21_string");
    TCase *tcase_core = tcase_create("core");
    
    // Добавляем тесты для memchr
    tcase_add_test(tcase_core, test_memchr_found);
    tcase_add_test(tcase_core, test_memchr_not_found);
    tcase_add_test(tcase_core, test_memchr_zero_n);
    tcase_add_test(tcase_core, test_memchr_boundary);
    
    // Добавляем тесты для memcmp
    tcase_add_test(tcase_core, test_memcmp_equal);
    tcase_add_test(tcase_core, test_memcmp_first_less);
    tcase_add_test(tcase_core, test_memcmp_zero_n);
    
    // Добавляем тесты для memcpy
    tcase_add_test(tcase_core, test_memcpy_basic);
    tcase_add_test(tcase_core, test_memcpy_zero_n);
    
    // Добавляем тесты для memset
    tcase_add_test(tcase_core, test_memset_basic);
    
    // Добавляем тесты для strlen
    tcase_add_test(tcase_core, test_strlen_basic);
    tcase_add_test(tcase_core, test_strlen_empty);
    
    // Добавляем тесты для strchr
    tcase_add_test(tcase_core, test_strchr_found);
    tcase_add_test(tcase_core, test_strchr_not_found);
    tcase_add_test(tcase_core, test_strchr_null_terminator);
    
    suite_add_tcase(suite, tcase_core);
    
    SRunner *runner = srunner_create(suite);
    srunner_run_all(runner, CK_NORMAL);
    int failed = srunner_ntests_failed(runner);
    srunner_free(runner);
    
    return failed ? 1 : 0;
}