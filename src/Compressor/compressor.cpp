/**
 * @file compressor.cpp
 * @brief Implementação das funções de compressão usando algoritmo de Huffman
 */

#include "compressor.hpp"
#include <bitset>

/**
 * @brief Carrega uma tabela de frequências a partir de um arquivo
 *
 * O arquivo deve estar no formato "caractere:frequência" com uma entrada por
 * linha. Linhas vazias ou começando com '#' são ignoradas.
 *
 * @param tablePath Caminho para o arquivo contendo a tabela de frequências
 * @return std::unordered_map<char, int> Mapa com os caracteres e suas
 * frequências
 * @throws std::runtime_error Se não for possível abrir o arquivo da tabela
 */
std::unordered_map<char, int>
Compressor::loadFrequencyTable(const std::string &tablePath) {
  std::unordered_map<char, int> freq;
  std::ifstream tableFile(tablePath);

  // Verifica se o arquivo foi aberto com sucesso
  if (!tableFile.is_open()) {
    throw std::runtime_error("Erro ao abrir tabela: " + tablePath);
  }

  std::string line;
  // Processa cada linha do arquivo
  while (std::getline(tableFile, line)) {
    // Ignora linhas vazias ou comentários
    if (line.empty() or line[0] == '#')
      continue;

    // Remove espaços em branco do início e fim da linha
    line.erase(0, line.find_first_not_of(" \t\r\n"));
    line.erase(line.find_last_not_of(" \t\r\n") + 1);

    if (line.empty())
      continue;

    // Encontra o separador ':' entre caractere e frequência
    size_t sep = line.find(':');
    if (sep == std::string::npos)
      continue;

    // Divide a linha em símbolo e frequência
    std::string symbolStr = line.substr(0, sep);
    std::string freqStr = line.substr(sep + 1);

    try {
      // Limpa espaços da string de frequência
      freqStr.erase(0, freqStr.find_first_not_of(" \t\r\n"));
      freqStr.erase(freqStr.find_last_not_of(" \t\r\n") + 1);

      // Converte e armazena os valores
      int count = std::stoi(freqStr);
      char symbol = symbolStr[0];
      freq[symbol] = count;
    } catch (...) {
      // Ignora linhas inválidas sem interromper o carregamento
      continue;
    }
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
  std::unordered_map<char, int> freq;

  // Decide de onde vem a tabela de frequências
  if (not tablePath.empty()) {
    // Tenta carregar tabela externa se fornecida
    try {
      std::cout << "Carregando tabela externa: " << tablePath << std::endl;
      freq = loadFrequencyTable(tablePath);
    } catch (...) {
      std::cerr << "Erro ao ler tabela externa" << std::endl;
    }
  } else {
    // Caso não haja tabela externa, gera automaticamente
    std::cout << "Gerando tabela automaticamente..." << std::endl;
    // falta colocara aqui boy caso vc queira criar esse caso aqui
  }
  
  if (freq.empty()) { //verifica se esta vazia 
    throw std::runtime_error("Tabela de frequências vazia!");
  }

  // Cria a árvore de Huffman a partir das frequências
  HuffmanTree tree(freq);
  auto codeTable = tree.getCodeTable();

  // Abre os arquivos de entrada e saída
  std::ifstream in(inputFile, std::ios::binary);
  std::ofstream out(outputFile, std::ios::binary);

  if (not in.is_open() or not out.is_open())
    throw std::runtime_error("Erro ao abrir arquivos para compressão.");

  // Escreve a tabela no início do arquivo .huf
  // Primeiro escreve o tamanho da tabela (número de entradas)
  uint32_t tableSize = freq.size();
  out.write(reinterpret_cast<char *>(&tableSize), sizeof(tableSize));

  // Escreve cada entrada da tabela (caractere + frequência)
  for (auto &[symbol, count] : freq) {
    out.put(symbol);
    out.write(reinterpret_cast<char *>(&count), sizeof(count));
  }

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