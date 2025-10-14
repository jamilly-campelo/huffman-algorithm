/**
 * @file huffman_tree.cpp
 * @brief Implementation of the Huffman tree for data compression
 */
#include "huffman_tree.hpp"
#include <queue>

/**
 * @struct NodeCompare
 * @brief Functor for node comparison in the priority queue
 *
 * Defines the order of nodes in the min-heap based on frequency.
 * Nodes with lower frequency have higher priority.
 */
struct NodeCompare {
  /**
   * @brief Comparison operator for Huffman tree nodes
   *
   * @param a First node to compare
   * @param b Second node to compare
   * @return true if the frequency of 'a' is greater than 'b' (for min-heap)
   */
  bool operator()(const std::shared_ptr<HuffmanNode> &a,
                  const std::shared_ptr<HuffmanNode> &b) const {
    return a->freq > b->freq;
  }
};

/**
 * @brief Constructor: builds the Huffman tree from character frequencies
 *
 * The algorithm builds the Huffman tree following these steps:
 * 1. Creates a leaf node for each character with its frequency
 * 2. Inserts all nodes into a min-heap (priority queue)
 * 3. While there is more than one node in the heap:
 *    - Removes the two nodes with the lowest frequency
 *    - Creates a new internal node with the sum of the frequencies
 *    - Inserts the new node into the heap
 * 4. The remaining node is the root of the tree
 * 5. Generates the code table by traversing the tree
 *
 * @param freq Unordered map containing characters and their frequencies
 *
 * @note Complexity: O(n log n) where n is the number of distinct characters
 * @note Characters with zero frequency are ignored
 */
HuffmanTree::HuffmanTree(const std::string &tablePath) {
  const std::unordered_map<std::string, int> freq = loadFrequencyTable(tablePath);
  // Priority queue (min-heap) to build the Huffman tree
  std::priority_queue<std::shared_ptr<HuffmanNode>,
                      std::vector<std::shared_ptr<HuffmanNode>>, NodeCompare>
      pq;

  // Create leaf nodes for each character and insert into the priority queue
  for (auto &[sym, f] : freq) {
    pq.push(std::make_shared<HuffmanNode>(sym, f));
  }

  // Build the tree by combining the two nodes with the lowest frequency until only one remains
  while (pq.size() > 1) {
    // Remove the two nodes with the lowest frequency
    auto left = pq.top(); pq.pop();
    auto right = pq.top(); pq.pop();

    // Create a new internal node with the sum of the frequencies
    pq.push(std::make_shared<HuffmanNode>(left, right));
  }

  // The remaining node is the root of the Huffman tree
  root = pq.top();

  // Build the code table by traversing the tree from the root
  buildCodes(root, "");
}

/**
 * @brief Recursively builds the Huffman code table
 *
 * Traverses the tree in pre-order assigning:
 * - '0' for left traversal
 * - '1' for right traversal
 *
 * @param node Current node being processed
 * @param code Binary code accumulated up to the current node
 *
 * @note When a leaf is reached, the accumulated code is stored in the table
 * @note The root has an empty code, which expands to the codes of the children
 */
void HuffmanTree::buildCodes(std::shared_ptr<HuffmanNode> node,
                             const std::string &code) {
  // Base case: null node, return without doing anything
  if (not node) return;

  // If it's a leaf node, store the code in the table
  if (node->isLeaf()) {
    codeTable[node->symbol] = code;
  } else {
    // Recursively traverse the left subtree adding '0' to the code
    buildCodes(node->left, code + "0");
    // Recursively traverse the right subtree adding '1' to the code
    buildCodes(node->right, code + "1");
  }
}

/**
 * @brief Returns the generated Huffman code table
 *
 * The table maps each character to its binary representation in the tree.
 * More frequent symbols have shorter codes.
 *
 * @return std::unordered_map<std::string, std::string> Encoding table
 *
 * @note The table is generated during tree construction
 * @note The code is a string of '0's and '1's
 * @note No code is a prefix of another (Huffman code property)
 */
std::unordered_map<std::string, std::string> HuffmanTree::getCodeTable() const {
  return codeTable;
}

std::shared_ptr<HuffmanNode> HuffmanTree::getRoot() const { return root; }

/**
 * @brief Loads a frequency table from a file
 *
 * The file must be in the format "string:frequency" with one entry per line.
 * Empty lines are ignored.
 *
 * @param tablePath Path to the file containing the frequency table
 * @return std::unordered_map<std::string, int> Map with symbols and their frequencies
 * @throws std::runtime_error If unable to open the table file
 */
std::unordered_map<std::string, int>
HuffmanTree::loadFrequencyTable(const std::string &tablePath) {
  std::unordered_map<std::string, int> freq;
  std::ifstream tableFile(tablePath);

  // Check if the file was opened successfully
  if (!tableFile.is_open()) {
    throw std::runtime_error("Error opening table: " + tablePath);
  }

  std::string line;
  // Process each line of the file
  while (std::getline(tableFile, line)) {
    // Ignore empty lines
    if (line.empty()) continue;

    size_t sep;
    // Find the ':' separator between character and frequency
    for (int i = line.size() - 1; i >= 0; i--) {
      if (line[i] == ':') {
        sep = i;
        break;
      }
    }

    // Split the line into symbol and frequency
    std::string symbolStr = line.substr(0, sep);
    std::string freqStr = line.substr(sep + 1);

    // Convert and store the values
    int count = std::stoi(freqStr);
    if(symbolStr.empty()) {
      freq["\n"] = count;
    } else {
      freq[symbolStr] = count;
    }
  }

  tableFile.close();

  // Add an end-of-file (EOF) symbol to handle padding
  freq["EOF"] = 1;

  return freq;
}