#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>

#define msToMinute(ms) ((ms/(1000*60))%60)
#define msToHours(ms) ((ms/(1000*60*60))%24)
#define msToSecond(ms) ((ms/1000)%60)

#define MINIMAL_PASSWORD_LENGTH 6
#define MAXIMAL_PASSWORD_LENGTH 8
#define HASH_LEN 50000
#define SHA256_SIZE_BYTE 32
#define SHA256_LENGTH 64
#define RTCHAIN_SIZE sizeof(rainbow::RTChain)

namespace rainbow
{

/**
 * @brief The rtChain struct Chain of a rainbowTable with the head and the tail
 */
struct RTChain
{
    char head[MAXIMAL_PASSWORD_LENGTH + 1];
    char tail[MAXIMAL_PASSWORD_LENGTH + 1];
};

constexpr const char * ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                  "abcdefghijklmnopqrstuvwxyz"
                                  "0123456789";

constexpr const char * FILE_NAME_RTABLE = "RainbowTable.txt";
constexpr const char * PASSWORD_CRACKED_FILE = "PasswordsCracked.txt";
constexpr int ALPHABET_SIZE = 62;
constexpr unsigned MB_TO_BYTE = 1048576;

constexpr float getSizeOnBytes(const float mb)
{
    return mb * MB_TO_BYTE;
}

constexpr float getSizeOnMegaBytes(const float bytes)
{
    return bytes /  MB_TO_BYTE;
}

} // end namespace rainbow

#endif // UTILS_HPP
