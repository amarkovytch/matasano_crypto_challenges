#include <iostream>

#include "gtest/gtest.h"
#include "utils.h"

TEST(TestEmpty, UtilsTests)
{
        auto empty = Utils::hexToBase64("");
        EXPECT_EQ(empty, "");
}