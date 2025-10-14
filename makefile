# ==============================================================================
# Makefile for Huffman Compressor and Frequency Table
# ==============================================================================

# Variables
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -g

OBJS_DIR := obj
BIN_DIR := bin

# --- Targets ---
SEMPRESS_EXEC := $(BIN_DIR)/sempress
FREQ_TABLE_EXEC := $(BIN_DIR)/freq-table

# --- Sources and Objects ---
SEMPRESS_SRCS := $(wildcard src/sempress/*.cpp)
SEMPRESS_OBJS := $(patsubst src/%.cpp,$(OBJS_DIR)/%.o,$(SEMPRESS_SRCS))

FREQ_TABLE_SRCS := $(wildcard src/table/*.cpp)
FREQ_TABLE_OBJS := $(patsubst src/%.cpp,$(OBJS_DIR)/%.o,$(FREQ_TABLE_SRCS))

.PHONY: all clean rebuild

all: $(SEMPRESS_EXEC) $(FREQ_TABLE_EXEC)

# --- Linking Rules ---
$(SEMPRESS_EXEC): $(SEMPRESS_OBJS)
	@mkdir -p $(BIN_DIR)
	@echo "🔗 Linking sempress executable..."
	$(CXX) $(CXXFLAGS) -o $@ $^
	@echo "✅ Executable 'sempress' created in $(BIN_DIR)!"
	@echo "Usage: ./$(SEMPRESS_EXEC) <table> <input> <output>"

$(FREQ_TABLE_EXEC): $(FREQ_TABLE_OBJS)
	@mkdir -p $(BIN_DIR)
	@echo "🔗 Linking frequency table executable..."
	$(CXX) $(CXXFLAGS) -o $@ $^
	@echo "✅ Executable 'freq-table' created in $(BIN_DIR)!"
	@echo "Usage: ./$(FREQ_TABLE_EXEC) <input_file_or_dir> [output_file]"

# --- Generic Compilation Rule ---
$(OBJS_DIR)/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	@echo "🔨 Compiling $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

# --- Utility Commands ---
clean:
	rm -rf $(OBJS_DIR) $(BIN_DIR)
	@echo "🧹 Clean up completed."

rebuild: clean all
