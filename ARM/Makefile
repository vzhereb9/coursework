CC=aarch64-linux-gnu-gcc
CFLAGS= -mcpu=cortex-a72 -O3 -c

all: coursework_Neon

coursework_Neon: main.o test_env.o calc.o recover.o basic_operators.o table_values.o
	$(CC) main.o test_env.o calc.o recover.o basic_operators.o table_values.o -lm -o coursework_Neon

main.o: main.c 
	$(CC) $(CFLAGS) main.c

test_env.o: test_env.c
	$(CC) $(CFLAGS) test_env.c

calc.o: calc.c
	$(CC) $(CFLAGS) calc.c

recover.o: recover.c
	$(CC) $(CFLAGS) recover.c

basic_operators.o: basic_operators.c
	$(CC) $(CFLAGS) basic_operators.c

table_values.o: table_values.c
	$(CC) $(CFLAGS) table_values.c

clean:
	rm -rf *.o coursework_Neon
