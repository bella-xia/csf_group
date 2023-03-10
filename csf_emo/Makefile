# Makefile

CC=gcc
CFLAGS=-std=c99 -pedantic -Wall -Wextra

# Links together files needed to create executable
csim: main.o input_check.o cache.o
	$(CC) -o csim main.o input_check.o cache.o -lm


# Compiles
main.o: main.c
	$(CC) $(CFLAGS) -c main.c

input_check.o: input_check.c input_check.h
	$(CC) $(CFLAGS) -c input_check.c

cache.o: cache.c cache.h
	$(CC) $(CFLAGS) -c cache.c

# Removes all object files and the executable named csim,
# so we can start fresh
clean:
	rm -f *.o csim
