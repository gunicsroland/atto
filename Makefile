CC      := gcc
CFLAGS  := -Wall -Wextra -pedantic -std=c99 -Iinclude
SRC_DIR := src
OBJ_DIR := obj
TARGET  := atto

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(CFLAGS)
	@echo "Build complete: $(TARGET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled: $< -> $@"

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)
	@echo "Created directory: $(OBJ_DIR)"

.PHONY: all clean