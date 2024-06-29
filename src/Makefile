.PHONY: all clean s21_string.a temp
CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11
SOURCES = $(wildcard *.c)
OBJECTS = $(patsubst %.c, %.o, $(SOURCES))
TEST = ./tests/test_s21_string.c
GCOV_FL = -fprofile-arcs -ftest-coverage -lcheck -lm -pthread
OS = $(shell uname)
ifeq ($(OS), Linux)
	TFLAGS = -lcheck -pthread -lrt -lm -lsubunit
else
	TFLAGS = -lcheck -coverage
endif

all:

s21_string.a: $(OBJECTS)
	ar rcs s21_string.a $(OBJECTS)
	ranlib s21_string.a

test_s21_string.o: $(TEST) s21_string.h
	checkmk tests/test.check > tests/test_s21_string.c
	$(CC) $(CFLAGS) -g -c $(TEST)

test: s21_string.a test_s21_string.o
	$(CC) $(CFLAGS) test_s21_string.o $(TFLAGS) s21_string.a -o test
	./test

gcov_report: s21_string.a test
	$(CC) $(CFLAGS) s21_string.a test_s21_string.o $(SOURCES) $(GCOV_FL) -o report
	./report
	lcov --capture --directory . --output-file coverage.info
	genhtml coverage.info --output-directory coverage-report
	open coverage-report/index.html

clean:
	rm -rf *.o *.a temp *.gcda *.gcno test test.dSYM report coverage-report coverage.info

rebuild:
	make clean
	make all