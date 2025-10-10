/**
 * @file file_reader.hpp
 * @brief Definição da classe FileReader para leitura de arquivos e carregamento
 * de frequências
 */

#ifndef FILE_READER_HPP
#define FILE_READER_HPP

#include <map>
#include <string>

/**
 * @class FileReader
 * @brief Classe utilitária para operações de leitura de arquivos e
 * processamento de frequências
 *
 * Esta classe fornece métodos estáticos para carregar frequências de
 * caracteres/palavras a partir de arquivos e para ler o conteúdo completo de
 * arquivos de texto. Todos os métodos são estáticos, não sendo necessário
 * instanciar a classe para uso.
 */
class FileReader {
public:
  /**
   * @brief Carrega frequências de strings a partir de um arquivo
   *
   * O arquivo deve conter pares no formato "string:frequência", um por linha.
   * Este método é útil para carregar tabelas de frequências previamente
   * calculadas.
   *
   * @param filename Caminho para o arquivo contendo as frequências
   * @param frequencies Map onde as frequências serão armazenadas (string ->
   * frequência)
   * @return true se o arquivo foi carregado com sucesso, false caso contrário
   *
   * @note O formato esperado é: "chave:valor" com uma entrada por linha
   * @note Linhas vazias ou mal formatadas são silenciosamente ignoradas
   * @note O map de frequências é limpo antes do carregamento
   *
   * @example
   * abacaxi:10
   * banana:15
   * laranja:8
   */
  static bool loadFrequencies(const std::string &filename,
                              std::map<std::string, int> &frequencies);

  /**
   * @brief Lê o conteúdo completo de um arquivo para uma string
   *
   * Este método abre o arquivo em modo binário e lê todo o seu conteúdo
   * para uma string, preservando todos os caracteres incluindo quebras de
   * linha.
   *
   * @param filename Caminho para o arquivo a ser lido
   * @param content String onde o conteúdo do arquivo será armazenado
   * @return true se o arquivo foi lido com sucesso, false caso contrário
   *
   * @note O arquivo é fechado automaticamente após a leitura
   * @note A string de conteúdo é limpa antes da leitura
   * @note Em caso de falha, a string content permanece vazia
   *
   * @throws Não lança exceções, retorna false em caso de erro
   */
  static bool readFileContent(const std::string &filename,
                              std::string &content);
};

#endif