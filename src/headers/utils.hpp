#ifndef FUNCUTILS_HPP
#define FUNCUTILS_HPP

#include <iostream>

#define msToMinute(ms) ((ms/(1000*60))%60)
#define msToHours(ms) ((ms/(1000*60*60))%24)
#define msToSecond(ms) ((ms/1000)%60)

namespace rainbow
{

constexpr const char * ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                  "abcdefghijklmnopqrstuvwxyz"
                                  "0123456789";

constexpr const char * FILE_NAME = "RainbowTable.txt";

constexpr const int ALPHABET_SIZE = 62;

inline void hexConvert(const char * text, unsigned char * bytes)
{
    unsigned int temp;
    int sha256_size = 32;
    for (int i = 0; i < sha256_size; i++)
    {
        std::sscanf(text + 2 * i, "%2x", &temp);
        bytes[i] = temp;
    }
}

constexpr float getSizeOnBytes(const float mb)
{
    const unsigned byteToMb = 1048576;
    return mb * byteToMb;
}

constexpr float getSizeOnMegaBytes(const float bytes)
{
    const unsigned byteToMb = 1048576;
    return bytes /  byteToMb;
}

} // end namespace rainbow

#endif // FUNCUTILS_HPP
