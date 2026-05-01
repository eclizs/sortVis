CC      = gcc
CFLAGS  = -Wall -Wextra -g -Iinclude

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Collect all .c files, map to obj/
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))
DEPS = $(OBJS:.o=.d)

TARGET = $(BIN_DIR)/main

# ── Default target ────────────────────────────────────────────────
all: $(TARGET)

# ── Link ──────────────────────────────────────────────────────────
$(TARGET): $(OBJS) | $(BIN_DIR)
		$(CC) $(CFLAGS) $^ -o $@

# ── Compile ───────────────────────────────────────────────────────
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
		$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

# ── Auto-create output dirs ───────────────────────────────────────
$(BIN_DIR) $(OBJ_DIR):
		mkdir -p $@

# ── Dependency includes ───────────────────────────────────────────
-include $(DEPS)

# ── Clean ─────────────────────────────────────────────────────────
.PHONY: all clean