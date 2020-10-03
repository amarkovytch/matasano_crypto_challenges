#include "byte_data.h"
#include "file_utils.h"
#include "matasano_asserts.h"
#include <iostream>
#include <set>
#include <string>

/**
 * @brief Iterates over the given vector of ByteData and finds a pair of identical ByteData (if any)
 *
 * @return pair of identical ByteData if found
 */
static std::optional<ByteData> hasIdenticalByteData(const std::vector<ByteData> &bytesVector)
{
    std::set<ByteData> bytesSet;
    for (auto &byteData : bytesVector)
    {
        if (bytesSet.contains(byteData))
        {
            return byteData;
        }

        bytesSet.insert(byteData);
    }

    return {};
}

int main()
{
    auto lines = FileUtils::readLines("assets/8.txt");
    bool found = false;
    std::optional<ByteData> identical;
    std::string ecbEncodedLine;
    std::string repeatingPattern;

    for (auto &line : lines)
    {
        auto rows = ByteData(line).extractRows(16);
        identical = hasIdenticalByteData(rows);

        if (identical)
        {
            // we should only hae one ECB enconded cipher, according to exercise
            VALIDATE_FALSE(found);
            found = true;
            ecbEncodedLine = line;
            repeatingPattern = (*identical).str();
        }
    }

    VALIDATE_NOT_EQ(repeatingPattern, "");

    std::cout << "This line is ECB encoded:" << std::endl;
    std::cout << ecbEncodedLine << std::endl;
    std::cout << "Because it has these 2 identical 16 byte blocks:" << std::endl;
    std::cout << (*identical).str() << std::endl;
}