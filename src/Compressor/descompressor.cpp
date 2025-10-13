/**
 * @file descompressor.cpp
 * @brief Implementação das funções de compressão usando algoritmo de Huffman
 */
#include "descompressor.hpp"
#include "huffman_tree.hpp"
#include <bitset>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iterator>

/**
 * @brief Comprime um arquivo usando codificação de Huffman
 *
 * A função pode usar uma tabela de frequências externa ou gerar uma
 * automaticamente. O arquivo de saída contém a tabela de frequências seguida
 * dos dados comprimidos.
 *
 * @param inputFile Caminho do arquivo de entrada a ser comprimido
 * @param outputFile Caminho do arquivo de saída comprimido
 * @param tablePath Caminho opcional para tabela de frequências externa
 * @throws std::runtime_error Se não for possível abrir os arquivos de
 * entrada/saída
 */
void Descompressor::decompress(const std::string &inputFile,
                const std::string &outputFile,
                const std::string &tablePath) {

  // Cria a árvore de Huffman a partir do arquivo de tabela
  HuffmanTree tree(tablePath);

  // Abre os arquivos de entrada e saída
  std::ifstream in(inputFile, std::ios::binary);
  std::ofstream out(outputFile);

  if (not in.is_open() or not out.is_open())
    throw std::runtime_error("Erro ao abrir arquivos para descompressão.");

  // 1. Lê o arquivo comprimido e o converte para uma string de bits
  std::string binary_string;
  char byte;
  while (in.get(byte)) {
    binary_string += std::bitset<8>(static_cast<unsigned char>(byte)).to_string();
  }
  in.close();

  // 2. Decodifica a string de bits usando a árvore de Huffman
  std::string decoded_string;
  std::shared_ptr<HuffmanNode> current_node = tree.getRoot();
  for (char bit : binary_string) {
    if (bit == '0') {
      current_node = current_node->left;
    } else {
      current_node = current_node->right;
    }

    // Se chegamos a um nó folha, encontramos um símbolo
    if (current_node->isLeaf()) {
      // Para a descompressão se o símbolo EOF for encontrado
      if (current_node->symbol == "EOF") {
        break;
      }
      decoded_string += current_node->symbol;
      current_node = tree.getRoot(); // Volta para a raiz para o próximo símbolo
    }
  }

  // 3. Escreve o conteúdo decodificado no arquivo de saída
  out << decoded_string;
  out.close();
}