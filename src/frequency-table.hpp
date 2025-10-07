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

std::unordered_map<std::string, int> create_unordered_map(std::string keywords_file, std::string ascii_file);

#endif