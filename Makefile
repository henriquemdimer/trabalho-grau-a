CC=gcc
CFLAGS=-Wall -Wextra -O3

BIN_DIR=bin
SRC=main.c
TARGET=$(BIN_DIR)/jogo

all: $(TARGET)

$(TARGET): $(BIN_DIR) $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

.PHONY: clean run dev

clean:
	@rm -rf $(BIN_DIR)

run:
	@./$(TARGET)

dev: $(TARGET) run
