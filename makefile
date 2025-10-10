# ==============================================================================
# Makefile para Compressor Huffman (localização flexível)
# ==============================================================================

# Variáveis
CXX := g++
TARGET := compressor
CXXFLAGS := -std=c++17 -Wall -Wextra -g

# Detecta automaticamente onde estão os arquivos
COMPRESSOR_DIR := $(shell find . -name "compressor.cpp" -type f | head -1 | xargs dirname)
ifeq ($(COMPRESSOR_DIR),)
    $(error Não encontrei a pasta do Compressor. Certifique-se de que compressor.cpp existe)
endif

OBJS_DIR := obj
BIN_DIR := bin

# Arquivos fonte
SRCS := $(COMPRESSOR_DIR)/main.cpp \
        $(COMPRESSOR_DIR)/compressor.cpp \
        $(COMPRESSOR_DIR)/huffman_tree.cpp

OBJS := $(patsubst $(COMPRESSOR_DIR)/%.cpp, $(OBJS_DIR)/%.o, $(SRCS))

.PHONY: all
all: $(BIN_DIR)/$(TARGET)

$(BIN_DIR)/$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	@echo "🔗 Ligando o executável..."
	$(CXX) $(CXXFLAGS) -o $@ $^
	@echo "✅ Executável '$(TARGET)' criado!"
	@echo "Arquivos compilados de: $(COMPRESSOR_DIR)"
	@echo "Uso: ./$(BIN_DIR)/$(TARGET) <tabela> <entrada> <saída>"

$(OBJS_DIR)/%.o: $(COMPRESSOR_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@echo "🔨 Compilando $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -rf $(OBJS_DIR) $(BIN_DIR)
	@echo "Limpeza concluída."

.PHONY: rebuild
rebuild: clean all