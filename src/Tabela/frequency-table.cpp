#include "frequency-table.hpp"

#include <iostream>
#include <filesystem>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <fstream>
#include <unordered_map>
namespace fs = std::filesystem;

std::string string_to_lower(const std::string& str) {
    std::string str_out;

    for (char c : str) str_out += tolower(c);

    return str_out;
}

void verifies_path(std::string arg, std::vector<std::string>& input_list) {    
    fs::path path = arg;
    fs::path path_lower = string_to_lower(arg);

    if (!fs::exists(path)) {
        std::cerr << "Sorry, unable to read \"" + arg + "\".\n";
        std::exit(2);
    } else if (fs::is_regular_file(path)) {
        if (path_lower.extension() == ".cpp") {
            input_list.push_back(arg);
        } else {
            if (path.extension().string().empty()) {
                std::cerr << "Sorry, file extension not identified.\n";
                std::exit(2);
            } else {
                std::cerr << "Sorry, \"" + path.extension().string() + "\" files are not supported at this time.\n";
                std::exit(2);
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

std::unordered_map<std::string, int> create_unordered_map_from_file(std::string file_path) {
    std::unordered_map<std::string, int> un_map;
    
    std::ifstream file(file_path);
    std::string line;

    while (std::getline(file, line)) {
        un_map.insert({line, 0});
    }

    return un_map;
}

std::vector<std::string> line_parser(const std::string str) {
    std::vector<std::string> tks;
    int idx = 0;

    for (int i = 0; i < str.size(); i++) {
        if (str[i] == ' ' || i == str.size() - 1) {
            std::string tk;
            
            if (str[i] == ' ') {
                tk = str.substr(idx, i - idx);
                tks.push_back(" ");
                idx = i + 1;
            } else {
                tk = str.substr(idx, i - idx + 1);
            }

            tks.push_back(tk);
        }
    }

    return tks;
}

std::string contains_keyword(int idx, const std::string& str, const std::unordered_map<std::string, int>& keywords_unordered_map) {
    std::string out = "";

    for (const auto& [word, count] : keywords_unordered_map) {
        if (str.compare(idx, word.length(), word) == 0) {
            out = word;
        }
    }

    return out;
}

void count_frequencies_in_file(const std::string path, std::unordered_map<std::string, int>& keywords_map, std::unordered_map<std::string, int>& chars_map) {
    std::ifstream file(path);
    std::string line;

    while (std::getline(file, line)) {
        std::vector<std::string> tks = line_parser(line);

        for (auto tk : tks) {
            if (chars_map.find(tk) != chars_map.end()) {
                chars_map[tk]++;
            } else {
                for (int i = 0; i < tk.length(); i++) {
                    std::string comparison = contains_keyword(i, tk, keywords_map);

                    if (comparison != "") {
                        keywords_map[comparison]++;
                        i += comparison.length();
                    } else {
                        std::string s(1, tk[i]);

                        if (chars_map.find(s) != chars_map.end()) {
                            chars_map[s]++;
                        } else {
                            chars_map[s] = 1;
                        }                        
                    }
                }
            }
        }
    }
}

void count_frequencies_in_various_files(const std::vector<std::string> input_list, std::unordered_map<std::string, int>& keywords_map, std::unordered_map<std::string, int>& chars_map) {
    for (auto file : input_list) {
        count_frequencies_in_file(file, keywords_map, chars_map);
    }
}

void create_frequency_table(const std::string path, const std::unordered_map<std::string, int>& keywords_map, const std::unordered_map<std::string, int>& chars_map) {
    std::ofstream file(path);

    for (const auto& [word, count] : chars_map) {
        file << word << ":" << count << '\n';
    }

    for (const auto& [word, count] : keywords_map) {
        file << word << ":" << count << '\n';
    }

    file.close();
}