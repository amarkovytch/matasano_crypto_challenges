#include "byte_data.h"
#include "internal/matasano_convert.h"
#include "matasano_asserts.h"

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
        throw std::runtime_error("constructing ByteData from encoding::base64 is currently not supported");
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

void ByteData::parsePlain(const std::string &str)
{
    LOGIC_ASSERT(byteData.size() == 0);

    for (unsigned char ch : str)
    {
        std::byte b{ch};
        byteData.push_back(b);
    }
}

void ByteData::parseHex(const std::string &str)
{
    LOGIC_ASSERT(byteData.size() == 0);

    checkHexIsEven(str);

    for (unsigned int i = 0; i < str.size(); i += 2)
    {
        auto number = Convert::parseNumFromStr(str.substr(i, 2), 16);
        LOGIC_ASSERT(number <= 255);
        std::byte b{static_cast<unsigned char>(number)};
        byteData.push_back(b);
    }
}

ByteData operator^(ByteData lhs, const ByteData &rhs)
{
    ByteData res(std::vector<std::byte>(lhs.byteData.size()));

    ByteData::xorVectors(lhs.byteData, rhs.byteData, res.byteData);

    return res;
}

ByteData &ByteData::operator^=(const ByteData &rhs)
{
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
    unsigned int i = 0;
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

    for (unsigned int i = 0; i < lhs.byteData.size(); i++)
    {
        if (lhs.byteData[i] != rhs.byteData[i])
        {
            return false;
        }
    }

    return true;
}