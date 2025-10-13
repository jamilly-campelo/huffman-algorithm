/**
 * @file compressor.cpp
 * @brief Implementação das funções de compressão usando algoritmo de Huffman
 */
#include "compressor.hpp"
#include "huffman_tree.hpp"
#include "../Tabela/frequency-table.hpp"
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
void Compressor::compress(const std::string &inputFile,
                          const std::string &outputFile,
                          const std::string &tablePath) {
  HuffmanTree tree(tablePath);
  auto codeTable = tree.getCodeTable();
  
  // Abre os arquivos de entrada e saída
  std::ifstream in(inputFile);
  std::ofstream out(outputFile, std::ios::binary);
  // std::ofstream out(outputFile);

  if (not in.is_open() or not out.is_open())
    throw std::runtime_error("Erro ao abrir arquivos para compressão.");

  // Lê todo o conteúdo do arquivo para a memória
  std::stringstream file_buffer;
  file_buffer << in.rdbuf();
  std::string file_content = file_buffer.str();
  in.close();

  // Extrai as chaves (tokens) da tabela de códigos e ordena por tamanho (maior para menor)
  std::vector<std::string> tokens;
  for (const auto &pair : codeTable) {
    tokens.push_back(pair.first);
  }
  std::sort(tokens.begin(), tokens.end(), [](const std::string &a, const std::string &b) {
    return a.length() > b.length();
  });
  
  std::string buffer; // Buffer para acumular bits
  size_t pos = 0;
  while (pos < file_content.length()) {
    std::string matched_token = "";
    // Encontra o maior token que corresponde à posição atual
    for (const auto &token : tokens) {
      if (file_content.substr(pos, token.length()) == token) {
        matched_token = token;
        break;
      }
    }

    if (!matched_token.empty()) {
      // Se um token foi encontrado, codifica o token
      buffer += codeTable.at(matched_token);
      std::cout << "codificando " << matched_token <<  " para " << codeTable.at(matched_token) << std::endl;
      pos += matched_token.length();
    } else {
      // Se nenhum token corresponde, codifica um único caractere
      std::string single_char(1, file_content[pos]);
      if (codeTable.count(single_char)) {
        buffer += codeTable.at(single_char);
        std::cout << "codificando " << single_char <<  " para " << codeTable.at(single_char) << std::endl;
      } else {
        std::cout << "olha um enter aqui gente " << std::endl;
      }
      pos++;
    }

    // Escreve bytes completos no arquivo de saída
    while (buffer.size() >= 8) {
      std::bitset<8> bits(buffer.substr(0, 8));
      buffer.erase(0, 8);
      out.put(static_cast<unsigned char>(bits.to_ulong()));
    }
  }

  // Codifica o símbolo de fim de arquivo (EOF)
  buffer += codeTable.at("EOF");

  // Processa bits restantes no buffer
  if (not buffer.empty()) {
    buffer.resize(8, '0'); // Garante que o buffer tenha 8 bits
    std::bitset<8> bits(buffer);
    out.put(static_cast<unsigned char>(bits.to_ulong()));
  }

  // Fecha o arquivo de saída
  out.close();

  std::cout << "Compressão concluída. Saída: " << outputFile << std::endl;
}