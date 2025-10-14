/**
 * @file decompressor.cpp
 * @brief Implementation of decompression functions using Huffman algorithm
 */
#include "decompressor.hpp"
#include "huffman_tree.hpp"
#include <bitset>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iterator>

/**
 * @brief Decompresses a file using the Huffman tree
 *
 * This function reads a compressed file, uses an external Huffman table
 * to reconstruct the tree, and decodes the compressed data, writing the
 * result to an output file.
 *
 * @param inputFile Path to the compressed file to be decompressed
 * @param outputFile Path to the decompressed output file
 * @param tablePath Path to the Huffman table used for decompression
 * @throws std::runtime_error If unable to open input/output files
 */
void Decompressor::decompress(const std::string &inputFile,
        const std::string &outputFile,
        const std::string &tablePath) {

  // Creates the Huffman tree from the table file
  HuffmanTree tree(tablePath);

  // Opens the input and output files
  std::ifstream in(inputFile, std::ios::binary);
  std::ofstream out(outputFile);

  if (not in.is_open() or not out.is_open())
  throw std::runtime_error("Error opening files for decompression.");

  // Reads the compressed file and converts it to a bit string
  std::string binary_string;
  char byte;
  while (in.get(byte)) {
    binary_string += std::bitset<8>(static_cast<unsigned char>(byte)).to_string();
  }
  in.close();

  // Decodes the bit string using the Huffman tree
  std::string decoded_string;
  std::shared_ptr<HuffmanNode> current_node = tree.getRoot();
  for (char bit : binary_string) {
    if (bit == '0') {
      current_node = current_node->left;
    } else {
      current_node = current_node->right;
    }

    // If we reach a leaf node, we found a symbol
    if (current_node->isLeaf()) {
      // Stop decompression if the EOF symbol is found
      if (current_node->symbol == "EOF") {
        break;
      }
      decoded_string += current_node->symbol;
      current_node = tree.getRoot(); // Go back to the root for the next symbol
    }
  }

  // Writes the decoded content to the output file
  out << decoded_string;
  out.close();
}