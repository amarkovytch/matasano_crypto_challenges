#include "general_utils.h"
#include "gtest/gtest.h"

TEST(SecureUtilsTest, WipeDataTest)
{
    uint8_t data[5] = {1, 2, 3, 4, 5};
    uint8_t zero[5] = {0};

    GeneralUtils::SecureWipeData(data, 5);
    ASSERT_EQ(0, memcmp(data, zero, 5));
}