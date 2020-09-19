#ifndef MATASANO_BYTE_DATA_H
#define MATASANO_BYTE_DATA_H

#include <cstddef>
#include <map>
#include <string>
#include <vector>

/**
 * @brief Class for holding byte data with a set of usefull operations
 */
class ByteData
{
public:
    /**
     * Enum type for string encoding that is received as input in various functions
     * or returned as output
     */
    enum class encoding
    {
        hex,    // hex encoding
        base64, // base64 encoding
        plain   // no encoding
    };

    /**
     * @brief Construct a new ByteData object from a string representation in
     * a given encoding
     *
     * @param str data to construct the object from
     * @param strEnc data encoding
     *
     * @throw std::invalid_argument if str is empty or does not correspond to given
     * encoding, or bad encoding is given
     * @throw std::runtime_error if encoding::base64 is used (currently unsupported)
     */
    ByteData(const std::string &str, encoding strEnc = encoding::hex);

    /**
     * @brief Construct a new ByteData object from a single byte
     *
     * @param b byte to construct object from
     */
    ByteData(std::byte b);

    /**
     * @brief Construct a new ByteData object from a vector of bytes
     *
     * @param bytes bytes to construct object from
     * @throw std::invalid_argument if bytes is empty
     */
    ByteData(std::vector<std::byte> bytes);

    /**
     * @brief concatenates the data of two objects
     *
     * @param lhs the first argument
     * @param rhs the second argument
     *
     * @return the result of the concatenation
     */
    friend ByteData operator+(ByteData lhs, const ByteData &rhs);

    /**
     * @brief concatenates other object to this one
     *
     * @param rhs the other object to concatenate
     *
     * @return reference to this object after concatenation
     */
    ByteData &operator+=(const ByteData &rhs);

    /**
     * @brief performs mathematical xor between two objects
     * rhs is applied cyclically to lhs
     *
     * @param lhs the first argument
     * @param rhs the second argument
     *
     * @return the result of the described xor operation
     */
    friend ByteData operator^(ByteData lhs, const ByteData &rhs);

    /**
     * @brief performs mathematical xor between this object and the other one
     * rhs is applied cyclically to lhs
     *
     * @param rhs the other object to perform xor operation
     *
     * @return this object that holds the result of the described xor operation
     */
    ByteData &operator^=(const ByteData &rhs);

    /**
     * @brief retrieve string representation of this object in given encoding
     *
     * @param strEnc encoding
     * @return string representation
     */
    std::string str(encoding strEnc = encoding::hex) const;

    /**
     * @brief checks whether two given ByteData objects hold identical data
     *
     * @param lhs the first argument
     * @param rhs the second argument
     *
     * @return true if objects hold the same data, false otherwise
     */
    friend bool operator==(const ByteData &lhs, const ByteData &rhs);

    /**
     * @brief checks whether two given ByteData objects do not hold identical data
     *
     * @param lhs the first argument
     * @param rhs the second argument
     *
     * @return true if objects do not hold the same data, false otherwise
     */
    friend inline bool operator!=(const ByteData &lhs, const ByteData &rhs)
    {
        return !(lhs == rhs);
    }

    /**
     * @brief Return reference to the undelying data vector
     *
     * @return reference to the data vector
     */
    inline std::vector<std::byte> const &data() const
    {
        return byteData;
    }

private:
    /**
     * underlying object byte data
     */
    std::vector<std::byte> byteData;

    /**
     * @brief parses string data as hex. Should be called from constructor
     *
     * @param hex hex data
     */
    void parseHex(const std::string &hex);

    /**
     * @brief parses string data as plain. Should be called from constructor
     *
     * @param plain hex data
     */
    void parsePlain(const std::string &plain);

    /**
     * @brief return string representation without any encoding
     *
     * @return string
     */
    std::string strPlain() const;

    /**
     * @brief return string representation in hex encoding
     *
     * @return string
     */
    std::string strHex() const;

    /**
     * @brief return string representation in base64 encoding
     *
     * @return string
     */
    std::string strBase64() const;

    /**
     * @brief xors two given vectors and stores the result in result
     * rhs is applied cyclically to lhs
     *
     * @param lhs the first argument
     * @param rhs the first argument
     * @param result the result to store, it's size should be the same
     * as size of lhs
     */
    static void xorVectors(const std::vector<std::byte> &lhs,
                           const std::vector<std::byte> &rhs,
                           std::vector<std::byte> &result);

    /**
     * @brief return string representation without any encoding
     * Internal function that does not need this
     *
     * @param data data vector
     *
     * @return string
     */
    static std::string strPlainInternal(const std::vector<std::byte> &data);

    /**
     * @brief return string representation in hex encoding of the data in range
     * [start, end)
     *
     * @param start starting index
     * @param end index one past the last index
     * @return string
     */
    std::string strHexInternal(std::size_t start, std::size_t end) const;
};

#endif
