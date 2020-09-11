#include "matasano_asserts.h"
#include <stdexcept>
#include <string>

void assertHexIsEven(const std::string hex)
{
    THROW_IF(hex.length() % 2 != 0, hex + " has uneven length",
             std::invalid_argument);
}