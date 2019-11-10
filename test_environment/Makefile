CC=gcc
CFLAGS=-c -Wall

all: test_environment

test_environment: main.o test_env.o
	$(CC) main.o test_env.o -o test_environment

main.o: main.c
	$(CC) $(CFLAGS) main.c

test_env.o: test_env.c
	$(CC) $(CFLAGS) test_env.c
