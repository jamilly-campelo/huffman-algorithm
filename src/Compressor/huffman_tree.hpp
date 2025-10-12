/**
 * @file huffman_tree.hpp
 * @brief Definição da estrutura HuffmanNode e classe HuffmanTree para
 * compressão de dados
 */

#pragma once
#include <fstream>
#include <memory>
#include <queue>
#include <string>
#include <unordered_map>

/**
 * @struct HuffmanNode
 * @brief Estrutura que representa um nó na árvore de Huffman
 *
 * Cada nó pode ser uma folha (contendo um símbolo) ou um nó interno
 * (contendo referências para subárvores esquerda e direita).
 */
struct HuffmanNode {
  char symbol; ///< Caractere armazenado no nó (apenas para folhas)
  int freq;    ///< Frequência do caractere ou soma das frequências dos filhos
  std::shared_ptr<HuffmanNode> left;  ///< Ponteiro para o filho esquerdo
  std::shared_ptr<HuffmanNode> right; ///< Ponteiro para o filho direito

  /**
   * @brief Construtor para nó folha
   *
   * Cria um nó folha contendo um símbolo e sua frequência.
   *
   * @param s Caractere a ser armazenado no nó
   * @param f Frequência do caractere
   */
  HuffmanNode(char s, int f)
      : symbol(s), freq(f), left(nullptr), right(nullptr) {}

  /**
   * @brief Construtor para nó interno
   *
   * Cria um nó interno que combina dois nós filhos. A frequência
   * do nó interno é a soma das frequências dos filhos.
   *
   * @param l Ponteiro para o nó filho esquerdo
   * @param r Ponteiro para o nó filho direito
   */
  HuffmanNode(std::shared_ptr<HuffmanNode> l, std::shared_ptr<HuffmanNode> r)
      : symbol(0), freq(l->freq + r->freq), left(l), right(r) {}

  /**
   * @brief Verifica se o nó é uma folha
   *
   * @return true se o nó não tem filhos, false caso contrário
   */
  bool isLeaf() const { return not left && not right; }
};

/**
 * @class HuffmanTree
 * @brief Implementa uma árvore de Huffman para compressão de dados
 *
 * A árvore de Huffman é uma árvore binária ótima para codificação
 * onde caracteres mais frequentes têm códigos mais curtos.
 *
 * @note A árvore segue a propriedade de prefixo: nenhum código é
 *       prefixo de outro código, permitindo decodificação única.
 */
class HuffmanTree {
private:
  std::shared_ptr<HuffmanNode> root; ///< Raiz da árvore de Huffman
  std::unordered_map<std::string, std::string>
      codeTable; ///< Tabela de codificação caractere->código

  /**
   * @brief Constrói a tabela de códigos percorrendo a árvore recursivamente
   *
   * Atribui '0' para ramificação à esquerda e '1' para ramificação à direita.
   *
   * @param node Nó atual sendo processado
   * @param code Código binário acumulado até o nó atual
   */
  void buildCodes(std::shared_ptr<HuffmanNode> node, const std::string &code);

public:
  /**
   * @brief Construtor padrão - cria uma árvore vazia
   */
  HuffmanTree() : root(nullptr) {}

  /**
   * @brief Construtor que cria a árvore a partir de uma tabela de frequências
   *
   * Constrói a árvore de Huffman ótima usando um algoritmo guloso baseado
   * em fila de prioridade (min-heap).
   *
   * @param freq Mapa não ordenado contendo caracteres e suas frequências
   */
  HuffmanTree(const std::unordered_map<std::string, int> &freq);

  /**
   * @brief Retorna a tabela de codificação gerada pela árvore
   *
   * @return std::unordered_map<char, std::string> Tabela que mapeia cada
   *         caractere para seu código binário correspondente
   *
   * @note Os códigos são strings contendo apenas '0' e '1'
   * @note Caracteres não presentes na árvore não estarão na tabela
   */
  std::unordered_map<std::string, std::string> getCodeTable() const;
};