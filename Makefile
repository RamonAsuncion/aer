SRC = src
OBJ = obj
BIN = bin

CFLAGS = -std=gnu99 -Wall -Wextra -g -lreadline

all: $(BIN)/ishell

$(BIN)/ishell: $(SRC)/ishell.c
	$(CC) $(CFLAGS) $^ -o $@

.PHONY: all clean
clean:
	rm -rf $(BIN)/*


