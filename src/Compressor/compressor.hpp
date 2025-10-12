/**
 * @file compressor.hpp
 * @brief Definição da classe Compressor para compactação de arquivos usando
 * algoritmo de Huffman
 */

#pragma once
#include "huffman_tree.hpp"
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

/**
 * @class Compressor
 * @brief Classe responsável pela compactação de arquivos usando codificação de
 * Huffman
 *
 * Esta classe implementa o algoritmo de compressão de Huffman, que utiliza
 * uma tabela de frequências para criar códigos de tamanho variável, onde
 * caracteres mais frequentes recebem códigos mais curtos.
 */
class Compressor {
public:
  /**
   * @brief Compacta um arquivo usando codificação de Huffman
   *
   * A função pode utilizar uma tabela de frequências externa ou gerar uma
   * automaticamente a partir do conteúdo do arquivo. O arquivo comprimido
   * resultante contém a tabela de frequências seguida dos dados codificados.
   *
   * @param inputFile Caminho para o arquivo de entrada a ser comprimido
   * @param outputFile Caminho para o arquivo de saída comprimido (normalmente
   * com extensão .huf)
   * @param tablePath Caminho opcional para arquivo de tabela de frequências
   * externa
   *
   * @note Se tablePath for vazio, a tabela de frequências será gerada
   * automaticamente
   * @note O arquivo de saída inclui a tabela de frequências no cabeçalho para
   * permitir descompressão
   *
   * @throws std::runtime_error Se não for possível abrir os arquivos de
   * entrada/saída
   * @throws std::exception Em caso de erro durante a leitura/escrita dos
   * arquivos
   */
  void compress(const std::string &inputFile, const std::string &outputFile,
                const std::string &tablePath = "");

private:
  /**
   * @brief Carrega uma tabela de frequências a partir de um arquivo de texto
   *
   * O arquivo deve seguir o formato:
   * - Uma entrada por linha no formato "caractere:frequência"
   * - Linhas começando com '#' são tratadas como comentários
   * - Linhas vazias são ignoradas
   * - Caracteres especiais devem ser escapados adequadamente
   *
   * @param tablePath Caminho para o arquivo contendo a tabela de frequências
   * @return std::unordered_map<char, int> Mapa contendo os caracteres e suas
   * respectivas frequências
   */
  std::unordered_map<std::string, int>
  loadFrequencyTable(const std::string &tablePath);
};