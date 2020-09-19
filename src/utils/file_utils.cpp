#include "file_utils.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

std::string FileUtils::readFile(const std::string &fileName)
{
    std::ifstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    file.open(fileName, std::ios::in | std::ios::binary);
    std::ostringstream contents;
    contents << file.rdbuf();
    file.close();
    return (contents.str());
}