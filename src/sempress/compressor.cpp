/**
 * @file compressor.cpp
 * @brief Implementation of compression functions using Huffman algorithm
 */
#include "compressor.hpp"
#include "huffman_tree.hpp"
#include "../table/frequency-table.hpp"
#include <bitset>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iterator>

/**
 * @brief Compresses a file using Huffman encoding
 *
 * The function uses an external frequency table. Thus, the output file
 * contains only the compressed data.
 *
 * @param inputFile Path to the input file to be compressed
 * @param outputFile Path to the compressed output file
 * @param tablePath Path to the external frequency table
 * @throws std::runtime_error If unable to open input/output files
 */
void Compressor::compress(const std::string &inputFile,
              const std::string &outputFile,
              const std::string &tablePath) {
  HuffmanTree tree(tablePath);
  auto codeTable = tree.getCodeTable();
  
  // Opens input and output files
  std::ifstream in(inputFile);
  std::ofstream out(outputFile, std::ios::binary);

  if (not in.is_open() or not out.is_open())
  throw std::runtime_error("Error opening files for compression.");

  // Reads the entire file content into memory
  std::stringstream file_buffer;
  file_buffer << in.rdbuf();
  std::string file_content = file_buffer.str();
  in.close();

  // Extracts the keys (tokens) from the code table and sorts by length (largest to smallest)
  std::vector<std::string> tokens;
  for (const auto &pair : codeTable) {
    tokens.push_back(pair.first);
  }

  std::sort(tokens.begin(), tokens.end(), [](const std::string &a, const std::string &b) {
    return a.length() > b.length();
  });
  
  std::string buffer; // Buffer to accumulate bits
  size_t pos = 0;
  while (pos < file_content.length()) {
    std::string matched_token = "";
    // Finds the largest token that matches the current position
    for (const auto &token : tokens) {
      if (file_content.substr(pos, token.length()) == token) {
        matched_token = token;
        break;
      }
    }

    if (!matched_token.empty()) {
      // If a token was found, encode the token
      buffer += codeTable.at(matched_token);
      pos += matched_token.length();
    } else {
      // If no token matches, encode a single character
      std::string single_char(1, file_content[pos]);
      if (codeTable.count(single_char)) {
        buffer += codeTable.at(single_char);
      }
      pos++;
    }

    // Writes complete bytes to the output file
    while (buffer.size() >= 8) {
      std::bitset<8> bits(buffer.substr(0, 8));
      buffer.erase(0, 8);
      out.put(static_cast<unsigned char>(bits.to_ulong()));
    }
  }

  // Encodes the end-of-file (EOF) symbol
  buffer += codeTable.at("EOF");

  // Processes remaining bits in the buffer
  if (not buffer.empty()) {
    buffer.resize(8, '0'); // Ensures the buffer has 8 bits
    std::bitset<8> bits(buffer);
    out.put(static_cast<unsigned char>(bits.to_ulong()));
  }

  // Closes the output file
  out.close();

  std::cout << "Compression completed. Output: " << outputFile << std::endl;
}
