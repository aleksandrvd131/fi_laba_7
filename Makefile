CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2
LDFLAGS =

all: main test_list

circular_list.o: circular_list.c circular_list.h
	$(CC) $(CFLAGS) -c circular_list.c

iterator.o: iterator.c iterator.h circular_list.h
	$(CC) $(CFLAGS) -c iterator.c

main.o: main.c circular_list.h iterator.h
	$(CC) $(CFLAGS) -c main.c

test_list.o: test_list.c circular_list.h iterator.h
	$(CC) $(CFLAGS) -c test_list.c

main: main.o circular_list.o iterator.o
	$(CC) $(CFLAGS) main.o circular_list.o iterator.o -o main $(LDFLAGS)

test_list: test_list.o circular_list.o iterator.o
	$(CC) $(CFLAGS) test_list.o circular_list.o iterator.o -o test_list $(LDFLAGS)

# Запуск тестов
test: test_list
	./test_list

# Запуск интерактивной программы
run: main
	./main

# Проверка на ошибки памяти через AddressSanitizer
asan: CFLAGS += -fsanitize=address,undefined -g
asan: LDFLAGS += -fsanitize=address,undefined
asan: clean all
	./test_list

# Проверка через Valgrind
valgrind: test_list
	valgrind --leak-check=full --error-exitcode=1 ./test_list

clean:
	rm -f *.o main test_list

.PHONY: all test run asan valgrind clean
