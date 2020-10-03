#ifndef MATASANO_DISTRIBUTION_H
#define MATASANO_DISTRIBUTION_H

#include "byte_data.h"

/**
* @brief Represents percentage distribution of each byte in a given ByteData
* Each byte can be queried for the percentage of number of time it appears in data as value.
* Bytes that do not appear in data, will have 0 percent.
* For example:
*
* If object has these bytes  : {10, 10, 99, 98}
* The resulting map shall be : {{10, 50}, {99, 25}, {98, 25}}
*/
class ByteDistribution
{
public:
    /**
     * @brief Construct a new Bytes Distribution object based on a given ByteData object
     *
     * @param byteData ByteData to construction distribution from
     */
    ByteDistribution(const ByteData &byteData);

    /**
     * @brief distribution of a particular byte in percentage. If byte did not appear in the ByteData object - return 0
     *
     * @param byte to check distribution for
     * @return distribution in percentage
     */
    inline double at(std::byte byte) const
    {
        return distributionMap_.contains(byte) ? distributionMap_.at(byte) : 0;
    }

    /**
     * @brief removes distribution of a particular byte. If it does not exists, there is no effect
     *
     * @param byte byte to remove
     */
    inline void erase(std::byte byte)
    {
        distributionMap_.erase(byte);
    }

    /**
     * @brief get the size of the distribution, i.e the number of members with non-zero percentage
     *
     * @return the size
     */
    inline std::size_t size() const
    {
        return distributionMap_.size();
    }

    /**
     * @brief returns non-negative double representing a 'distance' from another Distribution object
     * The closer this number to 0 the closer their distribution is
     *
     * @param anotherDistribution
     * @return double
     */
    double distance(const ByteDistribution &anotherDistribution) const;

private:
    /**
     * @brief the distribution map
     */
    std::map<std::byte, double> distributionMap_;
};

#endif