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
     * Enum type for string encoding that is received as input in various function or returned as output
     */
    enum class encoding
    {
        hex,    // hex encoding
        base64, // base64 encoding
        plain   // no encoding
    };

    /**
     * @brief Construct a new empty Byte Data object
     */
    ByteData() = default;

    /**
     * @brief Construct a new ByteData object from a string representation in a given encoding
     *
     * @param str data to construct the object from
     * @param strEnc data encoding
     *
     * @throw std::invalid_argument if str is empty or does not correspond to given encoding, or bad encoding is given
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
     * @brief performs mathematical xor between two objects rhs is applied cyclically to lhs
     * Both ByteData objects should be not empty
     *
     * @param lhs the first argument
     * @param rhs the second argument
     *
     * @return the result of the described xor operation
     * @throw std::invalid_argument if either of ByteData is empty
     */
    friend ByteData operator^(ByteData lhs, const ByteData &rhs);

    /**
     * @brief performs mathematical xor between this object and the other one rhs is applied cyclically to lhs
     * Both ByteData objects should be not empty
     *
     * @param rhs the other object to perform xor operation
     *
     * @return this object that holds the result of the described xor operation
     * @throw std::invalid_argument if either of ByteData is empty
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
    friend inline bool operator!=(const ByteData &lhs, const ByteData &rhs) { return !(lhs == rhs); }

    /**
     * @brief Return reference to the undelying data vector
     *
     * @return reference to the data vector
     */
    inline std::vector<std::byte> const &data() const { return byteData; }

    /**
     * @brief Return size of underlying ByteData vector
     *
     * @return size of ByteData
     */
    inline std::size_t size() const { return byteData.size(); }

    /**
     * @brief Find hamming distance with another ByteData
     *
     * @param another ByteData to compute distance with. Should be the same length as this one
     * @return double hamming distance normalized with the length of the byte data
     *
     * @throw std::invalid_argument if length of another is not equal to length of the current byte data
     */
    double hamming(const ByteData &another);

    /**
     * @brief Extract consequetive elements from byte data. Each 'elmsInRow' elements form one row. Stop after
     * 'maxRows', 0 - for iterating the whole byte data. The last row could have less elements than elmsInRow
     * This object should not be empty
     *
     * @param elmsInRow the number of bytes in one row (the maximum size of one ByteData in a vector). Should not be 0
     * @param maxRows the maximum number of rows to extract, 0 for no maximum (the maximum size of a vector)
     * @return vector of resulting rows of ByteData
     *
     * @throw std::invalid_argument if elmsInRow is 0, or this object is empty
     */
    std::vector<ByteData> extractRows(std::size_t elmsInRow, std::size_t maxRows = 0) const;

    /**
     * @brief Exctract each 'numColumns'-s element from byte data. For example if numColumns is 4 then this will be the
     * resulting vector (numbers are indexes of the elements in the original byte data vector):
     * {{0, 4, 8, ...}, {1, 5, 9, ...}, {2, 6, 10, ...}, {3, 7, 11, ...}}
     * The maximum number of elements in column is defined by 'maxElmsInColumn', 0 for unlimited (iterate all original
     * byte data vector)
     * The last columns may have less elements than first columns
     * This object should not be empty
     *
     * @param maxNumColumns the maximum number of columns to extract (the maximum size of a vector). Should not be 0.
     * @param maxElmsInColumn the maximum size of one ByteData in a vector
     * @return vector of resulting columns of ByteData
     *
     * @throw std::invalid_argument if numColumns is 0, or this object is empty
     */
    std::vector<ByteData> extractColumns(std::size_t maxNumColumns, std::size_t maxElmsInColumn = 0) const;

private:
    /**
     * underlying object byte data
     */
    std::vector<std::byte> byteData;

    /**
     * @brief parses string data as hex. Should be called from constructor
     *
     * @param hex hex data
     * @throw std::invalid_argument if hex string is not even size
     */
    void parseHex(const std::string &hex);

    /**
     * @brief parses string data as hex. Should be called from parseHex
     * The only difference from parseHex is that it can add to existing byte data
     *
     * @param hex hex data
     * @throw std::invalid_argument if hex string is not even size
     */
    void parseHexInternal(const std::string &hex);

    /**
     * @brief parses string data as plain. Should be called from constructor
     *
     * @param plain plain data
     */
    void parsePlain(const std::string &plain);

    /**
     * @brief parses string data as base64. Should be called from constructor
     *
     * @param base64 base64 data
     * @throw std::invalid_argument if base64 string is not multiply of 4
     */
    void parseBase64(const std::string &base64);

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
    static void xorVectors(const std::vector<std::byte> &lhs, const std::vector<std::byte> &rhs,
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
