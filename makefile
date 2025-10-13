# ==============================================================================
# Makefile para Compressor Huffman e Tabela de Frequência
# ==============================================================================

# Variáveis
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -g

OBJS_DIR := obj
BIN_DIR := bin

# --- Alvos ---
COMPRESSOR_EXEC := $(BIN_DIR)/compressor
FREQ_TABLE_EXEC := $(BIN_DIR)/freq-table

# --- Fontes e Objetos ---
COMPRESSOR_SRCS := $(wildcard src/Compressor/*.cpp)
COMPRESSOR_OBJS := $(patsubst src/%.cpp,$(OBJS_DIR)/%.o,$(COMPRESSOR_SRCS))

FREQ_TABLE_SRCS := $(wildcard src/Tabela/*.cpp)
FREQ_TABLE_OBJS := $(patsubst src/%.cpp,$(OBJS_DIR)/%.o,$(FREQ_TABLE_SRCS))

.PHONY: all clean rebuild

all: $(COMPRESSOR_EXEC) $(FREQ_TABLE_EXEC)

# --- Regras de Ligação ---
$(COMPRESSOR_EXEC): $(COMPRESSOR_OBJS)
	@mkdir -p $(BIN_DIR)
	@echo "🔗 Ligando o executável do compressor..."
	$(CXX) $(CXXFLAGS) -o $@ $^
	@echo "✅ Executável 'compressor' criado em $(BIN_DIR)!"
	@echo "Uso: ./$(COMPRESSOR_EXEC) <tabela> <entrada> <saída>"

$(FREQ_TABLE_EXEC): $(FREQ_TABLE_OBJS)
	@mkdir -p $(BIN_DIR)
	@echo "🔗 Ligando o executável da tabela de frequência..."
	$(CXX) $(CXXFLAGS) -o $@ $^
	@echo "✅ Executável 'freq-table' criado em $(BIN_DIR)!"
	@echo "Uso: ./$(FREQ_TABLE_EXEC) <entrada> <saída>"

# --- Regra de Compilação Genérica ---
$(OBJS_DIR)/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	@echo "🔨 Compilando $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

# --- Comandos Utilitários ---
clean:
	rm -rf $(OBJS_DIR) $(BIN_DIR)
	@echo "🧹 Limpeza concluída."

rebuild: clean all
