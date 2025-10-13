/**
 * @file main.cpp
 * @brief Programa principal para compressão de arquivos usando algoritmo de
 * Huffman
 */

#include "compressor.hpp"
#include "descompressor.hpp"
#include <iostream>

/**
 * @brief Função principal do programa de compressão
 *
 * Este programa permite comprimir arquivos usando codificação de Huffman,
 * com suporte a duas modalidades:
 * 1. Compressão com tabela de frequências externa
 * 2. Compressão automática (gera tabela a partir do arquivo)
 *
 * @param argc Número de argumentos da linha de comando
 * @param argv Array de argumentos da linha de comando
 * @return int Código de saída: 0 para sucesso, 1 para erro
 *
 * @note Modalidade 1: programa tabela entrada saida
 * @note Modalidade 2: programa entrada saida
 *
 * @throws std::exception Propaga exceções da classe Compressor
 */

void usage(char *argv[]) {
  std::cerr << "Uso:\n"
            << "  " << argv[0]
            << " <tabela_frequencia> <arquivo_entrada> <arquivo_saida> [-d : descomprimir]\n";
  std::exit(1);
}

 
int main(int argc, char *argv[]) {
  try {
    // Verifica número mínimo de argumentos
    if (argc < 4) {
      usage(argv);
    }

    std::string tablePath = argv[1];
    std::string inputFile = argv[2];
    std::string outputFile = argv[3];
  
    if(argc == 4) { 
      Compressor compressor;
      std::cout << "Compressão com tabela externa: " << tablePath << std::endl;
      compressor.compress(inputFile, outputFile, tablePath);
    } else {
      std::string flag = argv[4];
      if(flag != "-d") {
        usage(argv);
      }
      Descompressor descompressor;
      std::cout << "Descomprimindo ";
      descompressor.decompress(inputFile, outputFile, tablePath);
    }

  } catch (const std::exception &e) {
    std::cerr << "Erro: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}