CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11 -I.
CHECK_FLAGS = $(shell pkg-config --cflags --libs check)

NAME = s21_string.a
SRCS = s21_memchr.c s21_memcmp.c s21_memcpy.c s21_memset.c s21_strlen.c s21_strchr.c s21_strncat.c s21_strncmp.c s21_strncpy.c s21_sprintf.c
OBJS = $(SRCS:.c=.o)

TEST_SRC = test_runner.c
TEST_NAME = test_runner

# Цель по умолчанию
all: $(NAME)

# Обычная сборка библиотеки
$(NAME): $(OBJS)
	ar rcs $@ $^
	rm -f $(OBJS)

%.o: %.c s21_string.h
	$(CC) $(CFLAGS) -c $< -o $@

# Сборка и запуск тестов (без gcov)
test: $(NAME)
	$(CC) $(CFLAGS) $(TEST_SRC) $(NAME) $(CHECK_FLAGS) -o $(TEST_NAME)
	./$(TEST_NAME)

# Сборка с флагами gcov и генерация HTML-отчёта
gcov_report: clean
	$(CC) $(CFLAGS) --coverage $(SRCS) $(TEST_SRC) $(CHECK_FLAGS) -o $(TEST_NAME)
	./$(TEST_NAME)
	lcov --capture --directory . --output-file coverage.info --no-external
	genhtml coverage.info --output-directory gcov_report
	rm -f *.gcda *.gcno *.gcov coverage.info
	rm -f $(TEST_NAME)

# Очистка
clean:
	rm -f $(OBJS) $(TEST_NAME) *.gcda *.gcno *.gcov coverage.info
	rm -rf gcov_report

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re test gcov_report