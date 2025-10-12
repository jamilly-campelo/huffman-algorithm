/**
 * @file compressor.cpp
 * @brief Implementação das funções de compressão usando algoritmo de Huffman
 */
#include "compressor.hpp"
#include "../Tabela/frequency-table.hpp"
#include <bitset>

/**
 * @brief Carrega uma tabela de frequências a partir de um arquivo
 *
 * O arquivo deve estar no formato "caractere:frequência" com uma entrada por
 * linha. Linhas vazias são ignoradas.
 *
 * @param tablePath Caminho para o arquivo contendo a tabela de frequências
 * @return std::unordered_map<std::string, int> Mapa com os símbolos e suas
 * frequências
 * @throws std::runtime_error Se não for possível abrir o arquivo da tabela
 */
std::unordered_map<std::string, int>
Compressor::loadFrequencyTable(const std::string &tablePath) {
  std::unordered_map<std::string, int> freq;
  std::ifstream tableFile(tablePath);

  // Verifica se o arquivo foi aberto com sucesso
  if (!tableFile.is_open()) {
    throw std::runtime_error("Erro ao abrir tabela: " + tablePath);
  }

  std::string line;
  // Processa cada linha do arquivo
  while (std::getline(tableFile, line)) {
    // Ignora linhas vazias
    if (line.empty()) continue;

    size_t sep;
    // Encontra o separador ':' entre caractere e frequência
    for (int i = line.size() - 1; i >= 0; i--) {
      if (line[i] == ':') {
        sep = i;
        break;
      }
    }

    // Divide a linha em símbolo e frequência
    std::string symbolStr = line.substr(0, sep);
    std::string freqStr = line.substr(sep + 1);

    // Converte e armazena os valores
    int count = std::stoi(freqStr);
    freq[symbolStr] = count;
  }

  tableFile.close();
  return freq;
}

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
  std::unordered_map<std::string, int> freq;

  // Decide de onde vem a tabela de frequências
  if (not tablePath.empty()) {
    std::cout << "Carregando tabela externa: " << tablePath << std::endl;
    freq = loadFrequencyTable(tablePath);
  } else {
    // Caso não haja tabela externa, gera automaticamente
    std::cout << "Gerando tabela automaticamente..." << std::endl;
    
    std::unordered_map<std::string, int> keywords_map = create_unordered_map_from_file("../../inputs/cpp-keywords.txt");
    std::unordered_map<std::string, int> chars_map = create_unordered_map_from_file("../../inputs/ascii_chars.txt");

    count_frequencies_in_file(inputFile, keywords_map, chars_map);

    create_frequency_table("../../outputs/frequency-table.txt", keywords_map, chars_map);

    std::cout << "Frequency table created sucessfully in file \"frequency-table.txt\"\n";
  }

  // Cria a árvore de Huffman a partir das frequências
  HuffmanTree tree(freq);
  auto codeTable = tree.getCodeTable();

  // Abre os arquivos de entrada e saída
  std::ifstream in(inputFile, std::ios::binary);
  std::ofstream out(outputFile, std::ios::binary);

  if (not in.is_open() or not out.is_open())
    throw std::runtime_error("Erro ao abrir arquivos para compressão.");

  // Codifica os dados do arquivo de entrada
  std::string buffer; // Buffer para acumular bits antes de escrever em bytes
  char c;

  // Lê cada caractere do arquivo de entrada
  while (in.get(c)) {
    // Adiciona o código Huffman do caractere ao buffer
    buffer += codeTable[c];

    // Quando temos pelo menos 8 bits no buffer, escreve um byte no arquivo
    while (buffer.size() >= 8) {
      std::bitset<8> bits(buffer.substr(0, 8));
      buffer.erase(0, 8);
      out.put(static_cast<unsigned char>(bits.to_ulong()));
    }
  }

  // Processa bits restantes no buffer (último byte incompleto)
  if (not buffer.empty()) {
    // Completa com zeros à direita para formar um byte completo
    while (buffer.size() < 8)
      buffer += '0';
    std::bitset<8> bits(buffer);
    out.put(static_cast<unsigned char>(bits.to_ulong()));
  }

  // Fecha os arquivos
  in.close();
  out.close();

  std::cout << "Compressão concluída. Saída: " << outputFile << std::endl;
}