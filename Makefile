CC=gcc
CFLAGS=-I.
DEPS = lexico.h sintatico.h semantico.h gerador.h
OBJ = main.o lexico.o sintatico.o semantico.o gerador.o
EXEC = main

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(EXEC) *.o *~ core

