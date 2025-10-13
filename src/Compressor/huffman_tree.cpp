/**
 * @file huffman_tree.cpp
 * @brief Implementação da árvore de Huffman para compressão de dados
 */
#include "huffman_tree.hpp"
#include <queue>

/**
 * @struct NodeCompare
 * @brief Functor para comparação de nós na fila de prioridade
 *
 * Define a ordem dos nós na min-heap baseada na frequência.
 * Nós com menor frequência têm maior prioridade.
 */
struct NodeCompare {
  /**
   * @brief Operador de comparação para nós da árvore de Huffman
   *
   * @param a Primeiro nó a ser comparado
   * @param b Segundo nó a ser comparado
   * @return true se a frequência de 'a' é maior que a de 'b' (para min-heap)
   */
  bool operator()(const std::shared_ptr<HuffmanNode> &a,
                  const std::shared_ptr<HuffmanNode> &b) const {
    return a->freq > b->freq;
  }
};

/**
 * @brief Construtor: cria a árvore de Huffman a partir das frequências dos
 * caracteres
 *
 * O algoritmo constrói a árvore de Huffman seguindo estes passos:
 * 1. Cria um nó folha para cada caractere com sua frequência
 * 2. Insere todos os nós em uma min-heap (fila de prioridade)
 * 3. Enquanto houver mais de um nó na heap:
 *    - Remove os dois nós com menor frequência
 *    - Cria um novo nó interno com soma das frequências
 *    - Insere o novo nó na heap
 * 4. O nó restante é a raiz da árvore
 * 5. Gera a tabela de códigos percorrendo a árvore
 *
 * @param freq Mapa não ordenado contendo os caracteres e suas frequências
 *
 * @note Complexidade: O(n log n) onde n é o número de caracteres distintos
 * @note Caracteres com frequência zero são ignorados
 *
 * @example
 * Para frequências: {'a': 5, 'b': 9, 'c': 12, 'd': 13, 'e': 16, 'f': 45}
 * A árvore será construída combinando os nós de menor frequência primeiro
 */
HuffmanTree::HuffmanTree(const std::string &tablePath) {
  const std::unordered_map<std::string, int> freq = loadFrequencyTable(tablePath);
  // Fila de prioridade (min-heap) para construir a árvore de Huffman
  std::priority_queue<std::shared_ptr<HuffmanNode>,
                      std::vector<std::shared_ptr<HuffmanNode>>, NodeCompare>
      pq;

  // Cria nós folha para cada caractere e insere na fila de prioridade
  for (auto &[sym, f] : freq) {
    pq.push(std::make_shared<HuffmanNode>(sym, f));
  }

  // Constrói a árvore combinando os dois nós de menor frequência até restar apenas um
  while (pq.size() > 1) {
    // Remove os dois nós com menor frequência
    auto left = pq.top();
    pq.pop();
    auto right = pq.top();
    pq.pop();

    // Cria um novo nó interno com a soma das frequências
    pq.push(std::make_shared<HuffmanNode>(left, right));
  }

  // O nó restante é a raiz da árvore de Huffman
  root = pq.top();

  // Constrói a tabela de códigos percorrendo a árvore a partir da raiz
  buildCodes(root, "");
}

/**
 * @brief Constrói a tabela de códigos Huffman recursivamente
 *
 * Percorre a árvore em pré-ordem atribuindo:
 * - '0' para deslocamento à esquerda
 * - '1' para deslocamento à direita
 *
 * @param node Nó atual sendo processado
 * @param code Código binário acumulado até o nó atual
 *
 * @note Quando atinge uma folha, o código acumulado é armazenado na tabela
 * @note A raiz tem código vazio, que se expande para os códigos dos filhos
 *
 * @example
 * Se o caminho for esquerda->direita->esquerda a partir da raiz,
 * o código gerado será "010"
 */
void HuffmanTree::buildCodes(std::shared_ptr<HuffmanNode> node,
                             const std::string &code) {
  // Caso base: nó nulo, retorna sem fazer nada
  if (not node)
    return;

  // Se é um nó folha, armazena o código na tabela
  if (node->isLeaf()) {
    codeTable[node->symbol] = code;
  } else {
    // Percorre recursivamente a subárvore esquerda adicionando '0' ao código
    buildCodes(node->left, code + "0");
    // Percorre recursivamente a subárvore direita adicionando '1' ao código
    buildCodes(node->right, code + "1");
  }
}

/**
 * @brief Retorna a tabela de códigos Huffman gerada
 *
 * A tabela mapeia cada caractere para sua representação binária na árvore.
 * Caracteres mais frequentes têm códigos mais curtos.
 *
 * @return std::unordered_map<std::string, std::string> Tabela de codificação
 *
 * @note A tabela é gerada durante a construção da árvore
 * @note O código é uma string de '0's e '1's
 * @note Nenhum código é prefixo de outro (propriedade do código de Huffman)
 *
 * @example
 * Para a árvore exemplo, a tabela pode conter:
 * {'a': "1100", 'b': "1101", 'c': "100", 'd': "101", 'e': "111", 'f': "0"}
 */
std::unordered_map<std::string, std::string> HuffmanTree::getCodeTable() const {
  return codeTable;
}

std::shared_ptr<HuffmanNode> HuffmanTree::getRoot() const { return root; }

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
HuffmanTree::loadFrequencyTable(const std::string &tablePath) {
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
    if(symbolStr.empty()) {
      freq["\n"] = count;
    } else {
      freq[symbolStr] = count;
    }
  }

  tableFile.close();

  // Adiciona um símbolo de fim de arquivo (EOF) para lidar com o padding
  freq["EOF"] = 1;

  return freq;
}