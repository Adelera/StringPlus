#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "s21_string.h"

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

START_TEST(test_memset_basic) {
  char str1[10] = "Hello";
  char str2[10] = "Hello";
  s21_memset(str1, 'A', 3);
  memset(str2, 'A', 3);
  ck_assert_mem_eq(str1, str2, 5);
}
END_TEST

START_TEST(test_strlen_basic) {
  ck_assert_int_eq(s21_strlen("Hello"), strlen("Hello"));
}
END_TEST

START_TEST(test_strlen_empty) { ck_assert_int_eq(s21_strlen(""), strlen("")); }
END_TEST

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

START_TEST(test_strrchr_found) {
  char str[] = "Hello";
  ck_assert_ptr_eq(s21_strrchr(str, 'l'), strrchr(str, 'l'));
}
END_TEST

START_TEST(test_strrchr_not_found) {
  char str[] = "Hello";
  ck_assert_ptr_eq(s21_strrchr(str, 'z'), strrchr(str, 'z'));
}
END_TEST

START_TEST(test_strncmp_equal) {
  ck_assert_int_eq(s21_strncmp("Hello", "Hello", 5),
                   strncmp("Hello", "Hello", 5));
}
END_TEST

START_TEST(test_strncmp_less) {
  ck_assert_int_eq(s21_strncmp("Hello", "Helmo", 4),
                   strncmp("Hello", "Helmo", 4));
}
END_TEST

START_TEST(test_strncpy_basic) {
  char dest1[10] = {0};
  char dest2[10] = {0};
  s21_strncpy(dest1, "Hello", 5);
  strncpy(dest2, "Hello", 5);
  ck_assert_str_eq(dest1, dest2);
}
END_TEST

START_TEST(test_strncat_basic) {
  char dest1[20] = "Hi ";
  char dest2[20] = "Hi ";
  s21_strncat(dest1, "world", 3);
  strncat(dest2, "world", 3);
  ck_assert_str_eq(dest1, dest2);
}
END_TEST

START_TEST(test_strpbrk_found) {
  char str[] = "Hello World";
  ck_assert_ptr_eq(s21_strpbrk(str, "Wor"), strpbrk(str, "Wor"));
  ck_assert_ptr_eq(s21_strpbrk(str, "lo"), strpbrk(str, "lo"));
}
END_TEST

START_TEST(test_strpbrk_not_found) {
  char str[] = "Hello";
  ck_assert_ptr_eq(s21_strpbrk(str, "xyz"), strpbrk(str, "xyz"));
}
END_TEST

START_TEST(test_strstr_found) {
  char haystack[] = "Hello World";
  ck_assert_ptr_eq(s21_strstr(haystack, "World"), strstr(haystack, "World"));
  ck_assert_ptr_eq(s21_strstr(haystack, "lo"), strstr(haystack, "lo"));
}
END_TEST

START_TEST(test_strstr_not_found) {
  char haystack[] = "Hello";
  ck_assert_ptr_eq(s21_strstr(haystack, "xyz"), strstr(haystack, "xyz"));
}
END_TEST

START_TEST(test_strstr_empty_needle) {
  char haystack[] = "Hello";
  ck_assert_ptr_eq(s21_strstr(haystack, ""), strstr(haystack, ""));
}
END_TEST

START_TEST(test_strcspn_basic) {
  ck_assert_int_eq(s21_strcspn("abcde", "cd"), strcspn("abcde", "cd"));
  ck_assert_int_eq(s21_strcspn("Hello World", " "),
                   strcspn("Hello World", " "));
}
END_TEST

START_TEST(test_strcspn_no_match) {
  ck_assert_int_eq(s21_strcspn("abcde", "xyz"), strcspn("abcde", "xyz"));
}
END_TEST

START_TEST(test_strtok_basic) {
  char str1[] = "Hello,World,Test";
  char str2[] = "Hello,World,Test";
  char *delim = ",";

  char *token1 = s21_strtok(str1, delim);
  char *token2 = strtok(str2, delim);

  while (token1 && token2) {
    ck_assert_str_eq(token1, token2);
    token1 = s21_strtok(NULL, delim);
    token2 = strtok(NULL, delim);
  }
  ck_assert_ptr_eq(token1, token2);
}
END_TEST

START_TEST(test_strtok_multiple_delims) {
  char str1[] = "Hello  World\tTest";
  char str2[] = "Hello  World\tTest";
  char *delim = " \t";

  char *token1 = s21_strtok(str1, delim);
  char *token2 = strtok(str2, delim);

  while (token1 && token2) {
    ck_assert_str_eq(token1, token2);
    token1 = s21_strtok(NULL, delim);
    token2 = strtok(NULL, delim);
  }
  ck_assert_ptr_eq(token1, token2);
}
END_TEST

START_TEST(test_strerror_valid) {
  for (int i = 0; i < 13; i++) {
    ck_assert_str_eq(s21_strerror(i), strerror(i));
  }
}
END_TEST

START_TEST(test_strerror_invalid) {
  char *s21_result = s21_strerror(999);
  char expected[64];
  sprintf(expected, "Unknown error %d", 999);
  ck_assert_str_eq(s21_result, expected);
}
END_TEST

int main(void) {
  Suite *suite = suite_create("s21_string");
  TCase *tcase_core = tcase_create("core");

  // memchr
  tcase_add_test(tcase_core, test_memchr_found);
  tcase_add_test(tcase_core, test_memchr_not_found);
  tcase_add_test(tcase_core, test_memchr_zero_n);
  tcase_add_test(tcase_core, test_memchr_boundary);

  // memcmp
  tcase_add_test(tcase_core, test_memcmp_equal);
  tcase_add_test(tcase_core, test_memcmp_first_less);
  tcase_add_test(tcase_core, test_memcmp_zero_n);

  // memcpy
  tcase_add_test(tcase_core, test_memcpy_basic);
  tcase_add_test(tcase_core, test_memcpy_zero_n);

  // memset
  tcase_add_test(tcase_core, test_memset_basic);

  // strlen
  tcase_add_test(tcase_core, test_strlen_basic);
  tcase_add_test(tcase_core, test_strlen_empty);

  // strchr
  tcase_add_test(tcase_core, test_strchr_found);
  tcase_add_test(tcase_core, test_strchr_not_found);
  tcase_add_test(tcase_core, test_strchr_null_terminator);

  // strrchr
  tcase_add_test(tcase_core, test_strrchr_found);
  tcase_add_test(tcase_core, test_strrchr_not_found);

  // strncmp
  tcase_add_test(tcase_core, test_strncmp_equal);
  tcase_add_test(tcase_core, test_strncmp_less);

  // strncpy
  tcase_add_test(tcase_core, test_strncpy_basic);

  // strncat
  tcase_add_test(tcase_core, test_strncat_basic);

  // strpbrk
  tcase_add_test(tcase_core, test_strpbrk_found);
  tcase_add_test(tcase_core, test_strpbrk_not_found);

  // strstr
  tcase_add_test(tcase_core, test_strstr_found);
  tcase_add_test(tcase_core, test_strstr_not_found);
  tcase_add_test(tcase_core, test_strstr_empty_needle);

  // strcspn
  tcase_add_test(tcase_core, test_strcspn_basic);
  tcase_add_test(tcase_core, test_strcspn_no_match);

  // strtok
  tcase_add_test(tcase_core, test_strtok_basic);
  tcase_add_test(tcase_core, test_strtok_multiple_delims);

  // strerror
  tcase_add_test(tcase_core, test_strerror_valid);
  tcase_add_test(tcase_core, test_strerror_invalid);

  suite_add_tcase(suite, tcase_core);

  SRunner *runner = srunner_create(suite);
  srunner_run_all(runner, CK_NORMAL);
  int failed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return failed ? 1 : 0;
}