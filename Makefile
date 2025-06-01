CC=gcc
CFLAGS=-I.
DEPS = lexico.h sintatico.h
OBJ = main.o lexico.o sintatico.o
EXEC = main

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(EXEC) *.o *~ core

