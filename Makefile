CC = gcc

SRC = src/main.c

CFLAGS = -g -I/opt/homebrew/include -L/opt/homebrew/lib -lraylib

OUT_DIR = build

OUT = $(OUT_DIR)/main

all: $(OUT)

$(OUT_DIR):
	mkdir -p $(OUT_DIR)

$(OUT): $(OUT_DIR) $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

clean:
	rm -rf $(OUT_DIR)

.PHONY: all clean
