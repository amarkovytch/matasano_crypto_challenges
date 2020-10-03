#include "crypto_block_aggregator.h"
#include "matasano_asserts.h"
#include "padder.h"

CryptoBlockAggregator::CryptoBlockAggregator(const ByteData &source, Padding padding, std::size_t blockSize)
    : source_(source.extractRows(blockSize)), padding_(padding), blockSize_(blockSize)
{
    THROW_IF(padding != Padding::PadOnGetBlock && padding_ != Padding::UnpadOnAggregateBlock, "invalid padding",
             std::invalid_argument);
    THROW_IF(padding_ == Padding::UnpadOnAggregateBlock && source_.back().size() != blockSize_,
             "when Padding is UnpadOnAggregateBlock, source data should be whole blocks", std::invalid_argument);
}

ByteData CryptoBlockAggregator::getBlockFromSource()
{
    THROW_IF(!hasBlocksInSource(), "there are no more blocks to extract", std::runtime_error);
    THROW_IF(lastActionGet_, "can't peform getBlockFromSource twice", std::runtime_error);

    auto result = source_.at(currentIndex_);
    if (isLastIndex())
    {
        if (padding_ == Padding::PadOnGetBlock)
        {
            result = Padder::padToBlockSize(result, blockSize_);
        }
    }

    currentIndex_++;
    lastActionGet_ = true;

    return result;
}

void CryptoBlockAggregator::aggregateBlock(const ByteData &block)
{
    THROW_IF(!lastActionGet_, "can't peform aggregateOutput twice", std::runtime_error);
    THROW_IF(block.size() != blockSize_,
             "given input size " + std::to_string(block.size()) + " is not equal to block size " +
                 std::to_string(blockSize_),
             std::invalid_argument);

    auto result = block;

    // the last operation
    if (!hasBlocksInSource())
    {
        if (padding_ == Padding::UnpadOnAggregateBlock)
        {
            result = Padder::removePadding(result);
        }
    }

    lastActionGet_ = false;

    output_ += result;
}
