#include "frequency-table.hpp"

#include <iostream>
#include <vector>

int main(int argc, char* argv[]) {
    if (argc == 1) {
        std::cerr << "No file path provided. Please run the program again.\n";
        return 1;
    }

    std::vector<std::string> input_list;
    // std::string file_path = "/home/jamilly/Área de trabalho/programacao-competitiva";
    std::string file_path(argv[1]);

    verifies_path(file_path, input_list);

    std::unordered_map<std::string, int> keywords_map = create_unordered_map_from_file("../../inputs/cpp-keywords.txt");
    std::unordered_map<std::string, int> chars_map = create_unordered_map_from_file("../../inputs/ascii_chars.txt");

    count_frequencies_in_various_files(input_list, keywords_map, chars_map);

    create_frequency_table("../../outputs/frequency-table.txt", keywords_map, chars_map);

    std::cout << "Frequency table created sucessfully in file \"frequency-table.txt\"\n";

    return 0;
}