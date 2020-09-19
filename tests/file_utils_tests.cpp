#include "file_utils.h"
#include "gtest/gtest.h"
#include <fstream>
#include <string>
#include <vector>

TEST(FileUtilTest, TestRead) { ASSERT_EQ("Test12345", FileUtils::read("assets/file_read_test.txt")); }

TEST(FileUtilTest, TestReadNoFile) { ASSERT_THROW(FileUtils::read("blah.txt"), std::ifstream::failure); }

TEST(FileUtilTest, TestReadLinesNoFile) { ASSERT_THROW(FileUtils::readLines("blah.txt"), std::ifstream::failure); }

TEST(FileUtilTest, TestReadLinesFile)
{
    std::vector<std::string> expected{"Test1", "Test2", "Test3"};
    ASSERT_EQ(expected, FileUtils::readLines("assets/file_read_lines_text.txt"));
}