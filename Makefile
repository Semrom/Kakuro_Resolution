CC = gcc

LIBS = -lm
CCFLAGS = -Wall -ggdb

OBJ = grille.o csp.o backtrack.o util.o

all: main

main: main.c $(OBJ)
	$(CC) $(CCFLAGS) -o main main.c $(OBJ)

grille.o: grille.c
	$(CC) $(CCFLAGS) -c $^

csp.o: csp.c
	$(CC) $(CCFLAGS) -c $^

backtrack.o: backtrack.c
	$(CC) $(CCFLAGS) -c $^

util.o: util.c
	$(CC) $(CCFLAGS) -c $^

.PHONY : clean

clean:
	- rm -f $(OBJ)
	- rm -f main
