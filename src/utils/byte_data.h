#ifndef MATASANO_BYTE_DATA_H
#define MATASANO_BYTE_DATA_H

#include <botan/secmem.h>
#include <compare>
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
     * Enum type for string Encoding that is received as input in various function or returned as output
     */
    enum class Encoding
    {
        hex,    // hex Encoding
        base64, // base64 Encoding
        plain   // no Encoding
    };

    /**
     * @brief Construct a new empty Byte Data object
     */
    ByteData() = default;

    /**
     * @brief Construct a new ByteData object from a string representation in a given Encoding
     *
     * @param str data to construct the object from
     * @param strEnc data Encoding
     *
     * @throw std::invalid_argument if str does not correspond to given Encoding, or bad Encoding is given
     */
    ByteData(const std::string &str, Encoding strEnc = Encoding::hex);

    /**
     * @brief Construct a new ByteData object from a single byte
     *
     * @param b byte to construct object from
     */
    ByteData(std::uint8_t b);

    /**
     * @brief Construct a new Byte Data object with size 0's
     *
     * @param size the number of 0 to fill in underying data
     */
    explicit ByteData(std::size_t size) : ByteData(std::vector(size, std::uint8_t{0})){};

    /**
     * @brief Construct a new ByteData object from a vector of bytes
     *
     * @param bytes bytes to construct object from
     */
    ByteData(const std::vector<std::uint8_t> &bytes);

    /**
     * @brief Construct a new ByteData object from a vector of secure bytes
     *
     * @param bytes bytes to construct object from
     */
    ByteData(const Botan::secure_vector<std::uint8_t> &bytes);

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
     * @brief retrieve string representation of this object in given Encoding
     *
     * @param strEnc Encoding
     * @return string representation
     */
    std::string str(Encoding strEnc = Encoding::hex) const;

    /**
     * @brief auto generated three-way-comparison operator
     */
    auto operator<=>(const ByteData &) const = default;

    /**
     * @brief returns true if this Bytdata is equal 'cyclically' to the other one
     * Cyclically equal means that either this data appears in other cyclically, or vice vesrsa
     * For example :
     * {1, 2, 3}                   eqCyclically {1, 2, 3, 1, 2, 3, 1, 2, 3}    ==  true
     * {1, 2, 3, 1, 2, 3, 1, 2, 3} eqCyclically {1, 2, 3}                      ==  true
     * {1, 2, 3}                   eqCyclically {1, 2, 3, 1, 2, 3, 1, 2, 3, 1} ==  false
     *
     * For 2 empty vectors the result is true. When one vector is empty and the other is not - result is false
     *
     * @param other Other Bytedata to compare with
     * @return true if this and other ByteData are equall cyclically
     */
    bool eqCyclically(const ByteData &other) const;

    /**
     * @brief Return const reference to the undelying data vector
     *
     * @return const reference to the data vector
     */
    inline const Botan::secure_vector<std::uint8_t> &secureData() const { return byteData_; }

    /**
     * @brief Return reference to the undelying data vector
     *
     * @return reference to the data vector
     */
    inline Botan::secure_vector<std::uint8_t> &secureData() { return byteData_; }

    /**
     * @brief Return size of underlying ByteData vector
     *
     * @return size of ByteData
     */
    inline std::size_t size() const { return byteData_.size(); }

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
     * @brief Extracts sub ByteData from a given one
     *
     * @param start the index of element to start
     * @param count the number of elements to extract
     * @return sub ByteData
     *
     * @throw std::invalid_argument if start / count do not fall within the range of data in ByteData or if count is 0
     */
    ByteData subData(std::size_t start, std::size_t count) const;

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
    Botan::secure_vector<std::uint8_t> byteData_;

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
     * @brief return string representation without any Encoding
     *
     * @return string
     */
    std::string strPlain() const;

    /**
     * @brief return string representation in hex Encoding
     *
     * @return string
     */
    std::string strHex() const;

    /**
     * @brief return string representation in base64 Encoding
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
    static void xorVectors(const Botan::secure_vector<std::uint8_t> &lhs, const Botan::secure_vector<std::uint8_t> &rhs,
                           Botan::secure_vector<std::uint8_t> &result);

    /**
     * @brief return string representation without any Encoding
     * Internal function that does not need this
     *
     * @param data data vector
     *
     * @return string
     */
    static std::string strPlainInternal(const Botan::secure_vector<std::uint8_t> &data);

    /**
     * @brief return string representation in hex Encoding of the data in range
     * [start, end)
     *
     * @param start starting index
     * @param end index one past the last index
     * @return string
     */
    std::string strHexInternal(std::size_t start, std::size_t end) const;

    /**
     * @brief same as @eqCyclically but assumes lhs bigger or equal to rhs
     *
     * @param lhs left ByteData to compare (should be bigger or equal)
     * @param rhs right ByteData to compare
     * @return true if lhs and rhs ByteData are equall cyclically
     */
    bool eqCyclicallyInternal(const ByteData &lhs, const ByteData &rhs) const;
};

#endif
