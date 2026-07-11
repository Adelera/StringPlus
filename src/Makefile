CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11 -Wno-stringop-truncation
TEST_FLAGS = -lcheck -lsubunit -lpthread -lm -lrt
GCOV_FLAGS = -fprofile-arcs -ftest-coverage

SRCS = s21_memchr.c s21_memcmp.c s21_memcpy.c s21_memset.c \
       s21_strlen.c s21_strchr.c s21_strrchr.c s21_strncmp.c \
       s21_strncpy.c s21_strncat.c s21_strcspn.c s21_strpbrk.c \
       s21_strstr.c s21_strtok.c s21_strerror.c s21_sscanf.c

OBJS = $(SRCS:.c=.o)
TARGET = s21_string.a

TEST_SRC = test_runner.c
TEST_TARGET = test

all: $(TARGET)

$(TARGET): $(OBJS)
	ar rcs $@ $^
	ranlib $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TARGET)
	$(CC) $(CFLAGS) $(TEST_SRC) -L. -l:s21_string.a $(TEST_FLAGS) -o $(TEST_TARGET)
	./$(TEST_TARGET)

gcov_report:
	$(CC) $(CFLAGS) $(GCOV_FLAGS) $(SRCS) $(TEST_SRC) $(TEST_FLAGS) -o $(TEST_TARGET)
	./$(TEST_TARGET)
	lcov -t "test" -o test.info -c -d .
	genhtml -o report test.info
	rm -f *.gcda *.gcno

clean:
	rm -f $(OBJS) $(TARGET) $(TEST_TARGET) *.gcda *.gcno *.info
	rm -rf report

rebuild: clean all

.PHONY: all clean test gcov_report rebuild