#include "byte_data.h"
#include "internal/matasano_convert.h"
#include "matasano_asserts.h"
#include <bitset>

ByteData::ByteData(const std::string &str, encoding strEnc)
{
    THROW_IF(str.empty(), "can't construct from empty string", std::invalid_argument);

    switch (strEnc)
    {
    case encoding::hex:
        parseHex(str);
        break;
    case encoding::plain:
        parsePlain(str);
        break;
    case encoding::base64:
        parseBase64(str);
        break;
    default:
        throw std::invalid_argument("Bad encoding was given");
    }
}

ByteData::ByteData(std::byte b) { byteData.push_back(b); }

ByteData::ByteData(std::vector<std::byte> bytes)
{
    THROW_IF(bytes.empty(), "can't construct from empty string", std::invalid_argument);
    byteData = bytes;
}

ByteData operator+(ByteData lhs, const ByteData &rhs)
{
    ByteData res(lhs.byteData);
    res.byteData.insert(res.byteData.end(), rhs.byteData.begin(), rhs.byteData.end());
    return res;
}

ByteData &ByteData::operator+=(const ByteData &rhs)
{
    byteData.insert(byteData.end(), rhs.byteData.begin(), rhs.byteData.end());
    return *this;
}

void ByteData::parsePlain(const std::string &plain)
{
    LOGIC_ASSERT(byteData.size() == 0);

    for (unsigned char ch : plain)
    {
        std::byte b{ch};
        byteData.push_back(b);
    }
}

void ByteData::parseHex(const std::string &hex)
{
    LOGIC_ASSERT(byteData.size() == 0);

    parseHexInternal(hex);
}

void ByteData::parseHexInternal(const std::string &hex)
{
    THROW_IF(hex.length() % 2 != 0, hex + " has uneven length", std::invalid_argument);

    for (std::size_t i = 0; i < hex.size(); i += 2)
    {
        auto number = Convert::parseNumFromStr(hex.substr(i, 2), 16);
        LOGIC_ASSERT(number <= 255);
        std::byte b{static_cast<unsigned char>(number)};
        byteData.push_back(b);
    }
}

void ByteData::parseBase64(const std::string &base64)
{
    LOGIC_ASSERT(byteData.size() == 0);

    THROW_IF(base64.length() % 4 != 0, base64 + " has length that is not ultiply of 4", std::invalid_argument);

    auto eqPos = base64.find('=');
    THROW_IF(eqPos < base64.size() - 2, base64 + " is not in correct base64 format", std::invalid_argument);

    for (std::size_t i = 0; i < base64.size(); i += 4)
    {
        auto hex3 = Convert::base64_4ToHex3(base64.substr(i, 4));
        parseHexInternal(hex3);
    }

    // remove padding
    std::size_t numPad = 0;
    if (base64.at(base64.size() - 2) == '=')
    {
        THROW_IF(base64.back() != '=', base64 + " is not in correct base64 format", std::invalid_argument)
        numPad = 2;
    }
    else if ('=' == base64.back())
    {
        numPad = 1;
    }
    for (std::size_t i = 0; i < numPad; i++)
    {
        byteData.pop_back();
    }
}

ByteData operator^(ByteData lhs, const ByteData &rhs)
{
    THROW_IF(lhs.size() == 0, "lhs is empty", std::invalid_argument);
    THROW_IF(rhs.size() == 0, "rhs is empty", std::invalid_argument);

    ByteData res(std::vector<std::byte>(lhs.size()));

    ByteData::xorVectors(lhs.byteData, rhs.byteData, res.byteData);

    return res;
}

ByteData &ByteData::operator^=(const ByteData &rhs)
{
    THROW_IF(size() == 0, "this is empty", std::invalid_argument);
    THROW_IF(rhs.size() == 0, "rhs is empty", std::invalid_argument);

    xorVectors(byteData, rhs.byteData, byteData);

    return *this;
}

void ByteData::xorVectors(const std::vector<std::byte> &lhs, const std::vector<std::byte> &rhs,
                          std::vector<std::byte> &result)
{
    LOGIC_ASSERT(lhs.size() != 0);
    LOGIC_ASSERT(rhs.size() != 0);
    LOGIC_ASSERT(result.size() == lhs.size());

    for (unsigned i = 0; i < result.size(); i++)
    {
        result.at(i) = lhs.at(i) ^ rhs.at(i % rhs.size());
    }
}

std::string ByteData::str(encoding strEnc) const
{
    switch (strEnc)
    {
    case encoding::hex:
        return strHex();
        break;
    case encoding::plain:
        return strPlain();
        break;
    case encoding::base64:
        return strBase64();
        break;
    default:
        throw std::invalid_argument("Bad encoding was given");
    }
}

std::string ByteData::strPlain() const { return strPlainInternal(byteData); }

std::string ByteData::strPlainInternal(const std::vector<std::byte> &data)
{
    std::string res;

    for (std::byte b : data)
    {
        res += std::to_integer<unsigned char>(b);
    }

    return res;
}

std::string ByteData::strHex() const { return strHexInternal(0, byteData.size()); }

std::string ByteData::strHexInternal(std::size_t start, std::size_t end) const
{
    std::string res;

    for (auto i = start; i < end; i++)
    {
        res += Convert::numToStr(std::to_integer<unsigned char>(byteData.at(i)), 2, 16);
    }

    return res;
}

std::string ByteData::strBase64() const
{
    std::string result;

    auto numBytes = byteData.size();
    unsigned char bytesToPad = (3 - numBytes % 3) % 3;

    // convert every 3 bytes to 4 octal numbers and from that to one base64 symbol
    // iterate all the triples except the reminder
    std::size_t i = 0;
    for (; i < byteData.size() - byteData.size() % 3; i += 3)
    {
        result += Convert::hex3ToBase64_4(strHexInternal(i, i + 3));
    }

    if (bytesToPad != 0)
    {
        // convert the remaining 3 bytes with additional pad
        std::string paddedHexReminder = Convert::padWith(strHexInternal(i, byteData.size()), "00", bytesToPad);

        result += Convert::hex3ToBase64_4(paddedHexReminder);

        // remove the padding symbols
        result = result.erase(result.length() - bytesToPad, bytesToPad);

        result = Convert::padWith(result, "=", bytesToPad);
    }

    return result;
}

bool operator==(const ByteData &lhs, const ByteData &rhs)
{
    if (lhs.byteData.size() != lhs.byteData.size())
    {
        return false;
    }

    for (std::size_t i = 0; i < lhs.byteData.size(); i++)
    {
        if (lhs.byteData[i] != rhs.byteData[i])
        {
            return false;
        }
    }

    return true;
}

double ByteData::hamming(const ByteData &another)
{
    THROW_IF(byteData.size() != another.size(),
             "this object has length of " + std::to_string(byteData.size()) + " which is not equal to " +
                 std::to_string(another.size()),
             std::invalid_argument);

    if (size() == 0)
    {
        return 0;
    }

    auto tmp = (*this) ^ another;

    double byteFraction = 1.0 / (tmp.size());
    double result = 0;

    for (auto b : tmp.data())
    {
        std::bitset<8> bitset(std::to_integer<unsigned char>(b));
        result += bitset.count() * byteFraction;
    }

    return result;
}

std::vector<ByteData> ByteData::extractRows(std::size_t elmsInRow, std::size_t maxRows) const
{
    THROW_IF(size() == 0, "can't extract rows from empty object", std::invalid_argument);
    THROW_IF(0 == elmsInRow, "elmsInRow should not be 0", std::invalid_argument);

    std::vector<ByteData> result;
    ByteData currBD;
    std::size_t currRow = 0;
    std::size_t indexInRow = 0;

    for (std::size_t i = 0; i < byteData.size(); i++, indexInRow++)
    {
        if (indexInRow == elmsInRow)
        {
            indexInRow = 0;
            currRow++;
            result.push_back(currBD);
            currBD = ByteData();
        }

        if ((maxRows != 0) && (currRow >= maxRows))
        {
            break;
        }

        currBD += byteData.at(i);
    }

    if (currBD.size() != 0)
    {
        result.push_back(currBD);
    }

    return result;
}

std::vector<ByteData> ByteData::extractColumns(std::size_t maxNumColumns, std::size_t maxElmsInColumn) const
{
    THROW_IF(size() == 0, "can't extract columnts from empty object", std::invalid_argument);
    THROW_IF(0 == maxNumColumns, "maxNumColumns should not be 0", std::invalid_argument);

    std::vector<ByteData> result;
    std::size_t currColumn = 0;

    for (std::size_t i = 0; i < byteData.size(); i++, currColumn++)
    {
        if (currColumn == maxNumColumns)
        {
            currColumn = 0;
        }

        if (result.size() <= currColumn)
        {
            result.push_back(ByteData());
            LOGIC_ASSERT(result.size() > currColumn);
        }

        if (0 != maxElmsInColumn)
        {
            LOGIC_ASSERT(result.at(currColumn).size() <= maxElmsInColumn);
            if (result.at(currColumn).size() == maxElmsInColumn)
            {
                continue;
            }
        }

        result.at(currColumn) += byteData.at(i);
    }

    return result;
}