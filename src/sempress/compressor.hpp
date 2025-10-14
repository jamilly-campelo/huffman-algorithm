/**
 * @file compressor.hpp
 * @brief Definition of the Compressor class for file compression using
 * the Huffman Algorithm
 */
#pragma once
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

/**
 * @class Compressor
 * @brief Class responsible for file compression using Huffman encoding
 *
 * This class implements the Huffman compression algorithm, which uses
 * a frequency table to create variable-length codes, where more frequent
 * symbols receive shorter codes.
 */
class Compressor {
public:
  /**
   * @brief Compresses a file using Huffman encoding
   *
   * The function uses an external frequency table created previously.
   * The resulting compressed file contains only the encoded data.
   *
   * @param inputFile Path to the input file to be compressed
   * @param outputFile Path to the compressed output file
   * @param tablePath Path to the external frequency table file
   *
   * @throws std::runtime_error If unable to open input/output files
   * @throws std::exception In case of error during file reading/writing
   */
  void compress(const std::string &inputFile, const std::string &outputFile,
                const std::string &tablePath);
};
