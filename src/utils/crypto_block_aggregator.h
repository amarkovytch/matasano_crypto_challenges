#ifndef MATASANO_BLOCK_AGGREGATOR_H
#define MATASANO_BLOCK_AGGREGATOR_H

#include "byte_data.h"

/**
 * @brief The purpose of the class is to :
 * 1) Transforms ByteData from given source into separate blocks
 * 2) Aggregate separate blocks into one single ByteData
 * Both operations do padding and unpadding where needed.
 * The assumption is that this class is used during encryption / decryption operation to get block, decrypt / encrypt it
 * and store back. For each getBlockFromSource therefore, there should be an aggregateOutput
 */
class CryptoBlockAggregator
{
public:
    /**
     * @brief Denotes when to perform the padding
     */
    enum class Padding
    {
        PadOnGetBlock,        // pad data on extracting blocks (means that source is plain and we are encrypting it)
        UnpadOnAggregateBlock // unpad data on storing blocks (means that source is encrypted and we are decrypting it)
    };

    /**
     * @brief Construct a new Block Aggregator object
     *
     * @param source source of data to separate blocks, can't be empty
     * @param padding the padding direction
     * @param blockSize the block size
     *
     * @throw std::invalid_argument if source is empty or if invalid padding is supplied or if the blockSize is 0
     * Also on UnpadOnAggregateBlock if source data is not whole blocks (because in this case we assume that it is
     * padded encrypted data)
     */
    CryptoBlockAggregator(const ByteData &source, Padding padding, std::size_t blockSize = 16);

    /**
     * @brief Return true if there are still blocks to extract
     *
     * @return true if there are still blocks to return
     */
    inline bool hasBlocksInSource() const { return currentIndex_ < source_.size(); };

    /**
     * @brief Return the next block from source. If the block is the last one and Padding was PadOnGetBlock - pad it
     * This function should always be called after aggregateOutput, except the first time
     *
     * @return ByteData with the size of the block
     * @throw std::runtime_error if it was not called after aggregateOutput (except the first time), also after the last
     * block was returned
     */
    // TODO consider replacing with coroutines
    ByteData getBlockFromSource();

    /**
     * @brief Aggregates block to one single output ByteData. If this block is the last one (there are no more blocks in
     * source) and padding was UnpadOnAggregateBlock - pad it
     * This function should always be called after getBlockFromSource
     *
     * @param block to aggregate, should be block size
     * @throw std::runtime_error if it was not called after getBlockFromSource also after the last block was aggregated
     * @throw std::invalid_argument if input size is not equal to block size
     */
    void aggregateBlock(const ByteData &block);

    /**
     * @brief return aggregated ByteData
     *
     * @return aggregated output
     */
    inline ByteData const output() const { return output_; };

private:
    /**
     * @brief source divided into chucks of blocks size without any modification
     */
    std::vector<ByteData> source_;

    /**
     * @brief when to perform the padding
     */
    Padding padding_;

    /**
     * @brief the block size
     */
    std::size_t blockSize_;

    /**
     * @brief if true - last actin was getBlockFromSource. Inizialized to false to enable the first getBlockFromSource
     */
    bool lastActionGet_ = false;

    /**
     * @brief current index to extract from source
     */
    std::size_t currentIndex_ = 0;

    /**
     * @brief The gathered output
     */
    ByteData output_;

    /**
     * @brief return true if currentIndex points to the last index
     *
     * @return true if yes, false otherwise
     */
    inline bool isLastIndex() const { return currentIndex_ == (source_.size() - 1); };
};

#endif