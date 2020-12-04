#include "byte_data.h"
#include "internal/matasano_convert.h"
#include "matasano_asserts.h"
#include <bitset>

ByteData::ByteData(const std::string &str, Encoding strEnc)
{
    // THROW_IF(str.empty(), "can't construct from empty string", std::invalid_argument);

    switch (strEnc)
    {
    case Encoding::hex:
        parseHex(str);
        break;
    case Encoding::plain:
        parsePlain(str);
        break;
    case Encoding::base64:
        parseBase64(str);
        break;
    default:
        throw std::invalid_argument("Bad Encoding was given");
    }
}

ByteData operator+(ByteData lhs, const ByteData &rhs)
{
    ByteData res(lhs.byteData_);
    res.byteData_.insert(res.byteData_.end(), rhs.byteData_.begin(), rhs.byteData_.end());
    return res;
}

ByteData &ByteData::operator+=(const ByteData &rhs)
{
    byteData_.insert(byteData_.end(), rhs.byteData_.begin(), rhs.byteData_.end());
    return *this;
}

void ByteData::parsePlain(const std::string &plain)
{
    LOGIC_ASSERT(byteData_.size() == 0);

    for (unsigned char ch : plain)
    {
        byteData_.push_back(ch);
    }
}

void ByteData::parseHex(const std::string &hex)
{
    LOGIC_ASSERT(byteData_.size() == 0);

    parseHexInternal(hex);
}

void ByteData::parseHexInternal(const std::string &hex)
{
    THROW_IF(hex.length() % 2 != 0, hex + " has uneven length", std::invalid_argument);

    for (std::size_t i = 0; i < hex.size(); i += 2)
    {
        auto number = Convert::parseNumFromStr(hex.substr(i, 2), 16);
        LOGIC_ASSERT(number <= 255);
        std::uint8_t b{static_cast<std::uint8_t>(number)};
        byteData_.push_back(b);
    }
}

void ByteData::parseBase64(const std::string &base64)
{
    LOGIC_ASSERT(byteData_.size() == 0);

    if (base64.length() == 0)
    {
        return;
    }

    THROW_IF(base64.length() % 4 != 0, base64 + " has length that is not multiply of 4", std::invalid_argument);

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
        byteData_.pop_back();
    }
}

ByteData operator^(ByteData lhs, const ByteData &rhs)
{
    THROW_IF(lhs.size() == 0, "lhs is empty", std::invalid_argument);
    THROW_IF(rhs.size() == 0, "rhs is empty", std::invalid_argument);

    ByteData res = lhs;

    ByteData::xorVectors(lhs.byteData_, rhs.byteData_, res.byteData_);

    return res;
}

ByteData &ByteData::operator^=(const ByteData &rhs)
{
    THROW_IF(size() == 0, "this is empty", std::invalid_argument);
    THROW_IF(rhs.size() == 0, "rhs is empty", std::invalid_argument);

    xorVectors(byteData_, rhs.byteData_, byteData_);

    return *this;
}

bool ByteData::eqCyclically(const ByteData &other) const
{
    if (size() >= other.size())
    {
        return eqCyclicallyInternal(*this, other);
    }
    else
    {
        return eqCyclicallyInternal(other, *this);
    }
}

bool ByteData::eqCyclicallyInternal(const ByteData &lhs, const ByteData &rhs) const
{
    LOGIC_ASSERT(lhs.size() >= rhs.size());

    // means that rhs size is also 0
    if (lhs.size() == 0)
    {
        return true;
    }

    // here lhs size is not 0
    if (rhs.size() == 0)
    {
        return false;
    }

    if (lhs.size() % rhs.size() != 0)
    {
        return false;
    }

    for (std::size_t i = 0; i < lhs.size(); i++)
    {
        if (lhs.secureData().at(i) != rhs.secureData().at(i % rhs.size()))
        {
            return false;
        }
    }

    return true;
}

void ByteData::xorVectors(const Botan::secure_vector<std::uint8_t> &lhs, const Botan::secure_vector<std::uint8_t> &rhs,
                          Botan::secure_vector<std::uint8_t> &result)
{
    LOGIC_ASSERT(lhs.size() != 0);
    LOGIC_ASSERT(rhs.size() != 0);
    LOGIC_ASSERT(result.size() == lhs.size());

    for (unsigned i = 0; i < result.size(); i++)
    {
        result.at(i) = lhs.at(i) ^ rhs.at(i % rhs.size());
    }
}

std::string ByteData::str(Encoding strEnc) const
{
    switch (strEnc)
    {
    case Encoding::hex:
        return strHex();
        break;
    case Encoding::plain:
        return strPlain();
        break;
    case Encoding::base64:
        return strBase64();
        break;
    default:
        throw std::invalid_argument("Bad Encoding was given");
    }
}

std::string ByteData::strPlain() const { return strPlainInternal(byteData_); }

std::string ByteData::strPlainInternal(const Botan::secure_vector<std::uint8_t> &data)
{
    std::string res;

    for (std::uint8_t b : data)
    {
        res += b;
    }

    return res;
}

std::string ByteData::strHex() const { return strHexInternal(0, byteData_.size()); }

std::string ByteData::strHexInternal(std::size_t start, std::size_t end) const
{
    std::string res;

    for (auto i = start; i < end; i++)
    {
        res += Convert::numToStr(byteData_.at(i), 2, 16);
    }

    return res;
}

std::string ByteData::strBase64() const
{
    std::string result;

    auto numBytes = byteData_.size();
    auto bytesToPad = (3 - numBytes % 3) % 3;

    // convert every 3 bytes to 4 octal numbers and from that to one base64 symbol
    // iterate all the triples except the reminder
    std::size_t i = 0;
    for (; i < byteData_.size() - byteData_.size() % 3; i += 3)
    {
        result += Convert::hex3ToBase64_4(strHexInternal(i, i + 3));
    }

    if (bytesToPad != 0)
    {
        // convert the remaining 3 bytes with additional pad
        std::string paddedHexReminder = Convert::padWith(strHexInternal(i, byteData_.size()), "00", bytesToPad);

        result += Convert::hex3ToBase64_4(paddedHexReminder);

        // remove the padding symbols
        result = result.erase(result.length() - bytesToPad, bytesToPad);

        result = Convert::padWith(result, "=", bytesToPad);
    }

    return result;
}

double ByteData::hamming(const ByteData &another)
{
    THROW_IF(byteData_.size() != another.size(),
             "this object has length of " + std::to_string(byteData_.size()) + " which is not equal to " +
                 std::to_string(another.size()),
             std::invalid_argument);

    if (size() == 0)
    {
        return 0;
    }

    auto tmp = (*this) ^ another;

    double byteFraction = 1.0 / static_cast<double>(tmp.size());
    double result = 0;

    for (auto b : tmp.secureData())
    {
        std::bitset<8> bitset(b);
        result += static_cast<double>(bitset.count()) * byteFraction;
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

    for (std::size_t i = 0; i < byteData_.size(); i++, indexInRow++)
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

        currBD += byteData_.at(i);
    }

    if (currBD.size() != 0)
    {
        result.push_back(currBD);
    }

    return result;
}

ByteData ByteData::extractRow(std::size_t elmsInRow, std::size_t rowNum) const
{
    THROW_IF(size() == 0, "can't extract rows from empty object", std::invalid_argument);
    ByteData res;

    for (std::size_t i = elmsInRow * rowNum; (i < elmsInRow * rowNum + elmsInRow) && (i < size()); i++)
    {
        res += byteData_.at(i);
    }

    return res;
}

std::vector<ByteData> ByteData::extractColumns(std::size_t maxNumColumns, std::size_t maxElmsInColumn) const
{
    THROW_IF(size() == 0, "can't extract columnts from empty object", std::invalid_argument);
    THROW_IF(0 == maxNumColumns, "maxNumColumns should not be 0", std::invalid_argument);

    std::vector<ByteData> result;
    std::size_t currColumn = 0;

    for (std::size_t i = 0; i < byteData_.size(); i++, currColumn++)
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

        result.at(currColumn) += byteData_.at(i);
    }

    return result;
}

ByteData ByteData::subData(std::size_t start, std::size_t count) const
{
    THROW_IF((start + count) > size(),
             std::to_string(start) + " + " + std::to_string(count) +
                 " fall beyond the size of data:" + std::to_string(size()),
             std::invalid_argument);

    return ByteData(Botan::secure_vector<std::uint8_t>(byteData_.begin() + start, byteData_.begin() + start + count));
}

ByteData::ByteData(const std::vector<ByteData> &rows)
{
    for (auto const &byteData : rows)
    {
        *this += byteData;
    }
}