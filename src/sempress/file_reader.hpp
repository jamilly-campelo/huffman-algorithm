/**
 * @file file_reader.hpp
 * @brief Definition of the FileReader class for file reading and frequency loading
 */
#ifndef FILE_READER_HPP
#define FILE_READER_HPP

#include <map>
#include <string>

/**
 * @class FileReader
 * @brief Utility class for file reading operations and frequency processing
 *
 * This class provides static methods for loading character/word frequencies from files
 * and for reading the full content of text files. All methods are static, so
 * instantiation of the class is not required for use.
 */
class FileReader {
public:
  /**
   * @brief Loads string frequencies from a file
   *
   * The file must contain pairs in the format "string:frequency", one per line.
   * This method is useful for loading previously calculated frequency tables.
   *
   * @param filename Path to the file containing the frequencies
   * @param frequencies Map where the frequencies will be stored (string -> frequency)
   * @return true if the file was loaded successfully, false otherwise
   *
   * @note Expected format: "key:value" with one entry per line
   * @note Empty or malformed lines are silently ignored
   * @note The frequency map is cleared before loading
   */
  static bool loadFrequencies(const std::string &filename,
                              std::map<std::string, int> &frequencies);

  /**
   * @brief Reads the full content of a file into a string
   *
   * This method opens the file in binary mode and reads all its content
   * into a string, preserving all characters including line breaks.
   *
   * @param filename Path to the file to be read
   * @param content String where the file content will be stored
   * @return true if the file was read successfully, false otherwise
   *
   * @note The file is automatically closed after reading
   * @note The content string is cleared before reading
   * @note In case of failure, the content string remains empty
   *
   * @throws Does not throw exceptions, returns false on error
   */
  static bool readFileContent(const std::string &filename,
                              std::string &content);
};

#endif
