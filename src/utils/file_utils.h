#ifndef MATASANO_FILE_UTILS_H
#define MATASANO_FILE_UTILS_H

#include <string>

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
    static std::string readFile(const std::string &fileName);
};

#endif