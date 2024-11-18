CC = gcc -I ./include
CFLAGS = -std=gnu99 -Wall -g #-DDEBUG=1

INC = ./include
SRC = ./src
OBJ = ./obj
DOC = ./doc
BIN = ./bin

vpath %.h ./include
vpath %.c ./src

EXECS = ishell

all: mkpaths $(EXECS)

mkpaths:
	@ mkdir -p $(OBJ)
	@ mkdir -p $(BIN)
doc:
	doxygen

wrappers.o: wrappers.h
	@ mkdir -p $(OBJ)
	$(CC) $(CFLAGS) -c $(SRC)/wrappers.c -o $(OBJ)/wrappers.o

platform.o: platform.h
	@ mkdir -p $(OBJ)
	$(CC) $(CFLAGS) -c $(SRC)/platform.c -o $(OBJ)/platform.o

ishell: ishell.c wrappers.o platform.o
	@ mkdir -p $(BIN)
	$(CC) $(CFLAGS) $(OBJ)/wrappers.o $(OBJ)/platform.o $(SRC)/ishell.c -o $(BIN)/ishell -lreadline

.PHONY: clean
clean:
	/bin/rm -rf $(BIN)/* $(OBJ)/* core* *~ $(SRC)/*~ $(INC)/*~


