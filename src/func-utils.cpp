#include "src/headers/func-utils.hpp"
#include <fstream>
#include <iomanip>
#include <iostream>



namespace rainbow {

void hexConvert(const char *text, unsigned char *bytes)
{
    unsigned int temp;
    int sha256_size = 32;
    for (int i = 0; i < sha256_size; i++) {
        std::sscanf(text + 2 * i, "%2x", &temp);
        bytes[i] = temp;
    }
}

float getSizeOnBytes(const float mb)
{
    const unsigned byteToMb = 1048576;
    return mb * byteToMb;
}

float getSizeOnMegaBytes(const float bytes)
{
    const unsigned byteToMb = 1048576;
    return bytes /  byteToMb;
}

}
