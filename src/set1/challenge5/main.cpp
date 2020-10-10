#include "byte_data.h"
#include "file_utils.h"
#include "matasano_asserts.h"
#include <iostream>
#include <string>

static const std::string INPUT = "Burning 'em, if you ain't quick and nimble\n"
                                 "I go crazy when I hear a cymbal";

static const std::string KEY = "ICE";

static const std::string OUTPUT = "0b3637272a2b2e63622c2e69692a23693a2a3c6324202d623d63343c2a26226324272765272"
                                  "a282b2f20430a652e2c652a3124333a653e2b2027630c692b20283165286326302e27282f";

static const std::string LORD_OF_THE_RING_PART =
    "A final note may be added, on a point raised by several students of  the  lore of"
    "the period.On Thror's  Map  is  written  Here  of  old  was  Thrain  King  under  the  Mountain;"
    "yet  Thrain  was  the  son  of  Thror,  the  last  King  under  the  Mountain"
    "before  the  coming  of  the  dragon.  The  Map,  however,  is  not  in  error.  Names  are"
    "often  repeated  in  dynasties,  and  the  genealogies  show  that  a  distant  ancestor  of"
    "Thror  was  referred  to,  Thrain  I,  a  fugitive  from  Moria,  who  first  discovered  the"
    "Lonely Mountain, Erebor, and ruled there for a while, before his people moved on"
    "to the remoter mountains of the North. ";

int main()
{
    auto result = ByteData(INPUT, ByteData::Encoding::plain) ^ ByteData(KEY, ByteData::Encoding::plain);

    VALIDATE_STR_EQ(OUTPUT, result.str());

    std::cout << "Success !" << std::endl;

    std::cout << "Encrypting part pf the Lord of the Ring text with \"ICE\" key" << std::endl;
    std::cout << "Original text:" << std::endl;
    std::cout << LORD_OF_THE_RING_PART << std::endl;
    std::cout << std::endl;
    std::cout << "Result:" << std::endl;
    std::cout << (ByteData(LORD_OF_THE_RING_PART, ByteData::Encoding::plain) ^ ByteData(KEY, ByteData::Encoding::plain))
                     .str(ByteData::Encoding::plain)
              << std::endl;

    auto passwd = FileUtils::read("/etc/passwd");
    std::cout << std::endl << "etc password file :" << std::endl;
    std::cout << passwd << std::endl;
    std::cout << std::endl;
    std::cout << "Encypted result:" << std::endl;
    std::cout << (ByteData(passwd, ByteData::Encoding::plain) ^ ByteData(KEY, ByteData::Encoding::plain))
                     .str(ByteData::Encoding::plain)
              << std::endl;

    return 0;
}