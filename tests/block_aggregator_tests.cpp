#include "crypto_block_aggregator.h"
#include "padder.h"
#include "gtest/gtest.h"

TEST(CryptoBlockAggregator, TestInvalidCreate)
{
    ASSERT_THROW(CryptoBlockAggregator(ByteData(), CryptoBlockAggregator::Padding::PadOnGetBlock),
                 std::invalid_argument);
    ASSERT_THROW(CryptoBlockAggregator(ByteData("aaa"), CryptoBlockAggregator::Padding::PadOnGetBlock, 0),
                 std::invalid_argument);
}

TEST(CryptoBlockAggregator, TestInvalidAggregateSize)
{
    CryptoBlockAggregator aggregator(ByteData("1234567890"), CryptoBlockAggregator::Padding::PadOnGetBlock, 7);
    aggregator.blocksFromSource().begin();

    ASSERT_THROW(aggregator.aggregateBlock(ByteData("1")), std::invalid_argument);
}

TEST(CryptoBlockAggregator, TestInvalidUsage)
{
    CryptoBlockAggregator aggregator(ByteData("1234567890"), CryptoBlockAggregator::Padding::PadOnGetBlock, 7);
    ByteData block("1234567", ByteData::Encoding::plain);

    // can't call aggregateOutput as first operation
    ASSERT_THROW(aggregator.aggregateBlock(block), std::runtime_error);

    auto iter = aggregator.blocksFromSource().begin();

    // can't call iterator twice
    ASSERT_THROW(++iter, std::runtime_error);

    aggregator.aggregateBlock(block);
    // can't call aggregateOutput twice
    ASSERT_THROW(aggregator.aggregateBlock(block), std::runtime_error);
}

TEST(CryptoBlockAggregator, TestEmptyIteratorAfterEndReached)
{
    CryptoBlockAggregator aggregator(ByteData("1234567890", ByteData::Encoding::plain),
                                     CryptoBlockAggregator::Padding::PadOnGetBlock, 7);

    ByteData block("1234567", ByteData::Encoding::plain);

    for (const auto &block : aggregator.blocksFromSource())
    {
        aggregator.aggregateBlock(block);
    }

    ASSERT_THROW(aggregator.blocksFromSource(), std::runtime_error);
}

TEST(CryptoBlockAggregator, TestPadOnGetBlock)
{
    ByteData source("1234567890", ByteData::Encoding::plain);
    ByteData sourceLastBlockPadded = source + std::vector(4, std::byte{4});

    CryptoBlockAggregator aggregator(source, CryptoBlockAggregator::Padding::PadOnGetBlock, 7);

    for (const auto &block : aggregator.blocksFromSource())
    {
        ASSERT_EQ(7, block.size());

        aggregator.aggregateBlock(block);
    }

    ASSERT_EQ(sourceLastBlockPadded, aggregator.output());
}

TEST(CryptoBlockAggregator, TestSourceNotWholeBlocks)
{
    ByteData source("1234567890", ByteData::Encoding::plain);
    CryptoBlockAggregator(source, CryptoBlockAggregator::Padding::PadOnGetBlock, 7);
    ASSERT_THROW(CryptoBlockAggregator(source, CryptoBlockAggregator::Padding::UnpadOnAggregateBlock, 7),
                 std::invalid_argument);
}

TEST(CryptoBlockAggregator, TestUnpadOnAggregateBlock)
{
    ByteData source("1234567890", ByteData::Encoding::plain);
    auto padding = std::vector(4, std::byte{4});

    CryptoBlockAggregator aggregator(source + padding, CryptoBlockAggregator::Padding::UnpadOnAggregateBlock, 7);

    for (const auto &block : aggregator.blocksFromSource())
    {
        ASSERT_EQ(7, block.size());

        aggregator.aggregateBlock(block);
    }

    ASSERT_EQ(source, aggregator.output());
}