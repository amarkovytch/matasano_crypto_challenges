#include "file_utils.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

std::string FileUtils::read(const std::string &fileName)
{
    std::ifstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    file.open(fileName, std::ios::in | std::ios::binary);
    std::ostringstream contents;
    contents << file.rdbuf();
    file.close();
    return (contents.str());
}

std::vector<std::string> FileUtils::readLines(const std::string &fileName)
{
    auto str = read(fileName);
    auto result = std::vector<std::string>{};
    auto ss = std::stringstream{str};

    for (std::string line; std::getline(ss, line, '\n');)
    {
        result.push_back(line);
    }

    return result;
}