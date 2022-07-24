CC := gcc
CFLAGS := -Wall -O2

all: list.o

test: test_list
	./test_list

list.o: list.c list.h
	$(CC) $(CFLAGS) -c $< -o $@

test_list: test_list.c list.c list.h
	$(CC) $(CFLAGS) $^ -o $@

clean:
	$(RM) test_list list.o
