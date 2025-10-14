/**
 * @file main.cpp
 * @brief Main program for file compression using Huffman Algorithm
 */
#include "compressor.hpp"
#include "decompressor.hpp"
#include <iostream>

/**
 * @brief Main program function for compression
 *
 * This program allows compressing files using Huffman coding
 * with external frequency table
 *
 * @param argc Number of command line arguments
 * @param argv Array of command line arguments
 * @return int Exit code: 0 for success, 1 for error
 *
 * @throws std::exception Propagates exceptions from Compressor class
 */

void usage(char *argv[]) {
  std::cerr << "Usage:\n"
            << "  " << argv[0]
            << " <frequency_table> <input_file> <output_file> [-d : decompress]\n";
  std::exit(1);
}

int main(int argc, char *argv[]) {
  try {
    // Check minimum number of arguments
    if (argc < 4) {
      usage(argv);
    }

    std::string tablePath = argv[1];
    std::string inputFile = argv[2];
    std::string outputFile = argv[3];
  
    if(argc == 4) { 
      Compressor compressor;
      std::cout << "Starting compression...\n";
      compressor.compress(inputFile, outputFile, tablePath);
    } else {
      std::string flag = argv[4];
      if (flag != "-d") {
        usage(argv);
      }
      Decompressor decompressor;
      std::cout << "Starting decompression...\n";
      decompressor.decompress(inputFile, outputFile, tablePath);
    }

    std::cout << "Operation completed successfully.\n";

  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
