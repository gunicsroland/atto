CC      := gcc
CFLAGS  := -g -Wall -Wextra -pedantic -Iinclude -D_DEFAULT_SOURCE -D_BSD_SOURCE -D_GNU_SOURCE
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

clean:
	rm -rf $(OBJ_DIR) $(TARGET)

# Run clang-format (in-place)
format:
	clang-format -i $(SRCS) include/*.h

# Run clang-tidy
tidy:
	clang-tidy $(SRCS) -- -Iinclude $(CFLAGS)

.PHONY: all clean format tidy