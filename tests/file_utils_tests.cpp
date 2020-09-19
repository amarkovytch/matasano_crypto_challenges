#include "file_utils.h"
#include "gtest/gtest.h"
#include <fstream>

TEST(FileUtilTest, TestRead)
{
    ASSERT_EQ("Test12345", FileUtils::readFile("assets/file_read_test.txt"));
}

TEST(FileUtilTest, TestReadNoFile)
{
    ASSERT_THROW(FileUtils::readFile("blah.txt"), std::ifstream::failure);
}
