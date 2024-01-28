CC = gcc
CFLAGS = -Wall -pedantic -std=gnu99 -Werror -I/local/courses/csse2310/include
LDFLAGS = -L/local/courses/csse2310/lib -lcsse2310a1
DEBUG = -g

.DEFAULT_GOAL := uqwordladder
.PHONY: debug clean

debug: CFLAGS += $(DEBUG)
debug: clean uqwordladder

uqwordladder.o: uqwordladder.c
	$(CC) $(CFLAGS) -c $^ -o $@

uqwordladder: uqwordladder.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

clean:
	rm -f uqwordladder *.o
