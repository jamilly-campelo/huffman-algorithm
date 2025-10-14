/**
 * @file decompressor.hpp
 * @brief Definition of the Decompressor class for decompressing files using the Huffman algorithm
 */
#pragma once
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

/**
 * @class Decompressor
 * @brief Class responsible for decompressing files using Huffman coding
 *
 * This class implements the Huffman decompression algorithm, which uses a frequency table to decode
 * variable-length codes, where more frequent characters have shorter codes.
 */
class Decompressor {
public:
  /**
   * @brief Decompresses a file using Huffman coding
   *
   * This function uses an external frequency table previously created. The compressed file should
   * contain only the encoded data.
   *
   * @param inputFile Path to the input file to be decompressed
   * @param outputFile Path to the output decompressed file
   * @param tablePath Path to the external frequency table file
   *
   * @throws std::runtime_error If unable to open input/output files
   * @throws std::exception In case of errors during file reading/writing
   */
  void decompress(const std::string &inputFile, const std::string &outputFile,
                  const std::string &tablePath);
};
