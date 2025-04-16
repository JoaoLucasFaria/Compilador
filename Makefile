CC=gcc
CFLAGS=-I.
DEPS = lexico.h
OBJ = main.o lexico.o
EXEC = main

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(EXEC) *.o *~ core

