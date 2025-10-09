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

void verifies_path(std::string arg, std::vector<std::string>& input_list);

std::unordered_map<std::string, int> create_unordered_map_from_file(std::string file_path);

std::vector<std::string> line_parser(const std::string str);

void count_frequencies_in_various_files(const std::vector<std::string> input_list, std::unordered_map<std::string, int>& keywords_map, std::unordered_map<std::string, int>& chars_map);

void create_frequency_table(const std::string path, const std::unordered_map<std::string, int>& keywords_map, const std::unordered_map<std::string, int>& chars_map);

#endif