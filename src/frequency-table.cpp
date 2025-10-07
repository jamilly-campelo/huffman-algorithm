#include "frequency-table.hpp"

#include <iostream>
#include <filesystem>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <fstream>
#include <unordered_map>
namespace fs = std::filesystem;

std::string string_to_lower(std::string str) {
    std::string str_out;

    for (char c : str) str_out += tolower(c);

    return str_out;
}

void insert_file(std::string file, std::vector<std::string>& input_list) {
    auto it = std::find(input_list.begin(), input_list.end(), file);

    if (it == input_list.end()) {
        input_list.push_back(file);
    }
}

void verifies_path(std::string arg, std::vector<std::string>& input_list) {    
    fs::path path = arg;
    fs::path path_lower = string_to_lower(arg);

    if (!fs::exists(path)) {
        std::cerr << "Sorry, unable to read \"" + arg + "\".\n";
    } else if (fs::is_regular_file(path)) {
        if (path_lower.extension() == ".cpp") {
            input_list.push_back(arg);
        } else {
            if (path.extension().string().empty()) {
                std::cerr << "Sorry, file extension not identified.\n";
            } else {
                std::cerr << "Sorry, \"" + path.extension().string() + "\" files are not supported at this time.\n";
            }
        }                
    } else if (fs::is_directory(path)) {
        for (const auto& entry : fs::recursive_directory_iterator(path)) {
            if (fs::is_regular_file(entry) && string_to_lower(entry.path().extension().string()) == ".cpp") {
                input_list.push_back(entry.path().string());
            }
        }
    }
}

std::unordered_map<std::string, int> create_unordered_map(std::string keywords_file, std::string ascii_file) {
    std::unordered_map<std::string, int> un_map;
    
    std::ifstream file(keywords_file);
    std::string line;

    while (std::getline(file, line)) {
        un_map.insert({line, 0});
    }

    file.close();
    file.open(ascii_file);

    while (std::getline(file, line)) {
        un_map.insert({line, 0});
    }

    return un_map;
}

void count_frequencies_in_file(std::string path, std::unordered_map<std::string, int> un_map) {
    
}