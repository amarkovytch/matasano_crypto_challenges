#include <coroutine>

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

CryptoBlockAggregator::Iterator CryptoBlockAggregator::blocksFromSource()
{
    THROW_IF(lastElementInSourceReached_, "there are no more blocks to extract", std::runtime_error);

    if (padding_ == Padding::PadOnGetBlock)
    {
        source_.back() = Padder::padToBlockSize(source_.back(), blockSize_);
    }

    lastActionGet_ = true;

    return CryptoBlockAggregator::Iterator(source_.begin(), *this);
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
    if (lastElementInSourceReached_)
    {
        if (padding_ == Padding::UnpadOnAggregateBlock)
        {
            result = Padder::removePadding(result);
        }
    }

    lastActionGet_ = false;

    output_ += result;
}

void CryptoBlockAggregator::Iterator::operator++()
{
    THROW_IF(parent_.lastActionGet_, "you didn't call aggregateBlock in last iteration !", std::runtime_error);
    iterator_++;

    if (iterator_ == parent_.source_.begin() + parent_.source_.size() - 1)
    {
        parent_.lastElementInSourceReached_ = true;
    }

    parent_.lastActionGet_ = true;
}