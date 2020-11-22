.PHONY: all
all:
	gcc -Wall -O2 -c -o list.o list.c

.PHONY: test
test:
	gcc -Wall -O2 -o test_list test_list.c
	./test_list

.PHONY: clean
clean:
	rm -f test_list list.o
