CC=gcc
CFLAGS=-I.
DEPS = lfsr.h
OBJ = lfsr.o main.o 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

lfsrcaller: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)

run-example:
	./lfsrcaller 0 1 6 7
