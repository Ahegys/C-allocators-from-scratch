PROG = prog

SRC_DIR = src
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/objs
BIN_DIR = $(BUILD_DIR)/bin

CC = gcc
CFLAGS =-fsanitize=address -Wall -Wextra -O2 -I./src/

SRCS = $(wildcard $(SRC_DIR)/*.c) index.c
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/src/%.o,$(filter $(SRC_DIR)/%.c,$(SRCS))) \
       $(patsubst %.c,$(OBJ_DIR)/%.o,$(filter %.c,$(SRCS)))

all: $(BIN_DIR)/$(PROG)

$(BIN_DIR)/$(PROG): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ_DIR)/src/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(BUILD_DIR)

# Phony targets
.PHONY: all clean