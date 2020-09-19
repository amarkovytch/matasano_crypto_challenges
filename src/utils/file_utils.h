#ifndef MATASANO_FILE_UTILS_H
#define MATASANO_FILE_UTILS_H

#include <string>
#include <vector>

class FileUtils
{
public:
    /**
     * @brief Reads the contents of a given filename
     *
     * @param fileName filename or path to read from
     * @return file contents as string
     *
     * @throw std::ifstream::failure on any failure during reading the file
     */
    static std::string read(const std::string &fileName);

    /**
     * @brief Reads the contents of a given filename and returns a vector of lines
     *
     * @param fileName filename or path to read from
     * @return vector of string lines
     *
     * @throw std::ifstream::failure on any failure during reading the file
     */
    static std::vector<std::string> readLines(const std::string &fileName);
};

#endif