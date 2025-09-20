SRC = src
BIN = bin

CFLAGS = -Wall -Wextra -g

all: $(BIN)/aer

$(BIN)/aer: $(SRC)/aer.c
	$(CC) $(CFLAGS) $^ -o $@

.PHONY: all clean
clean:
	rm -rf $(BIN)/*


