/**
 * @file frequency-table.hpp
 * @brief Function declarations for frequency table generation.
 */

#include <iostream>
#include <filesystem>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <fstream>
#include <unordered_map>
namespace fs = std::filesystem;

#ifndef FREQUENCY_TABLE_HPP
#define FREQUENCY_TABLE_HPP

/**
 * @brief Verifies the input path and populates a list of .cpp files.
 * @param arg Path to file or directory.
 * @param input_list Vector to store valid .cpp file paths.
 */
void verifies_path(std::string arg, std::vector<std::string>& input_list);

/**
 * @brief Creates an unordered map from a file, initializing values to zero.
 * @param file_path Path to the input file.
 * @return Unordered map with keys from file and values set to zero.
 */
std::unordered_map<std::string, int> create_unordered_map_from_file(std::string file_path);

/**
 * @brief Parses a line into tokens, separating by spaces.
 * @param str Input string.
 * @return Vector of tokens.
 */
std::vector<std::string> line_parser(const std::string str);

/**
 * @brief Counts frequencies of keywords and characters in a file.
 * @param path Path to the file.
 * @param keywords_map Map of keywords and their frequencies.
 * @param chars_map Map of characters and their frequencies.
 */
void count_frequencies_in_file(const std::string path, std::unordered_map<std::string, int>& keywords_map, std::unordered_map<std::string, int>& chars_map);

/**
 * @brief Counts frequencies in multiple files.
 * @param input_list List of file paths.
 * @param keywords_map Map of keywords and their frequencies.
 * @param chars_map Map of characters and their frequencies.
 */
void count_frequencies_in_various_files(const std::vector<std::string> input_list, std::unordered_map<std::string, int>& keywords_map, std::unordered_map<std::string, int>& chars_map);

/**
 * @brief Creates a frequency table and writes it to a file.
 * @param path Output file path.
 * @param keywords_map Map of keywords and their frequencies.
 * @param chars_map Map of characters and their frequencies.
 */
void create_frequency_table(const std::string path, const std::unordered_map<std::string, int>& keywords_map, const std::unordered_map<std::string, int>& chars_map);

#endif