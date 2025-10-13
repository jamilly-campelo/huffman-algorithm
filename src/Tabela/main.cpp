#include "frequency-table.hpp"

#include <iostream>
#include <vector>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file_or_dir> [output_file]" << std::endl;
        std::cerr << "  <input_file_or_dir>: Path to a .cpp file or a directory to scan for .cpp files." << std::endl;
        std::cerr << "  [output_file]:       Optional. Path to save the frequency table. Defaults to ../../outputs/frequency-table.txt" << std::endl;
        return 1;
    }

    std::vector<std::string> input_list;
    // std::string file_path = "/home/jamilly/Área de trabalho/programacao-competitiva";
    std::string file_path(argv[1]);

    verifies_path(file_path, input_list);

    std::unordered_map<std::string, int> keywords_map = create_unordered_map_from_file("inputs/cpp-keywords.txt");
    std::unordered_map<std::string, int> chars_map = create_unordered_map_from_file("inputs/ascii_chars.txt");

    count_frequencies_in_various_files(input_list, keywords_map, chars_map);

    if(argc == 3) {
        create_frequency_table(argv[2], keywords_map, chars_map);
        std::cout << "Frequency table created sucessfully in file " << "\"" << argv[2] << "\"\n";
    } else {
        create_frequency_table("outputs/frequency-table.txt", keywords_map, chars_map);
        std::cout << "Frequency table created sucessfully in file \"outputs/frequency-table.txt\"\n";
    }


    return 0;
}