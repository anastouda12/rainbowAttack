#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <thread>

namespace rainbow
{

    static const unsigned num_cpus =
            std::thread::hardware_concurrency(); // number optimal thread

    constexpr const char *ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                     "abcdefghijklmnopqrstuvwxyz"
                                     "0123456789";
    constexpr const unsigned PASSWORD_SIZE = 8;
    constexpr const unsigned HASH_LEN = 50000;
    constexpr const unsigned SHA256_SIZE_BYTE = 32;
    constexpr const unsigned SHA256_LENGTH = 64;
    constexpr const char *FILE_NAME_RTABLE = "RainbowTable.txt";
    constexpr const char *PASSWORD_CRACKED_FILE = "PasswordsCracked.txt";
    constexpr int ALPHABET_SIZE = 62;
    constexpr unsigned MB_TO_BYTE = 1048576;

    /**
     * @brief The rtChain struct Chain of a rainbowTable with the head and the tail
     */
    struct RTChain
    {
        char head[PASSWORD_SIZE + 1];
        char tail[PASSWORD_SIZE + 1];
    };
    constexpr const unsigned RTCHAIN_SIZE = sizeof(rainbow::RTChain);

    constexpr bool isEmptyChain(RTChain chain)
    {
        return chain.head[0] != '\0' && chain.tail[0] != '\0';
    }

    constexpr float getSizeOnBytes(const float mb)
    {
        return mb * MB_TO_BYTE;
    }

    constexpr float getSizeOnMegaBytes(const float bytes)
    {
        return bytes /  MB_TO_BYTE;
    }


    constexpr void copyArrays(const char *src, char *dest)
    {
        for (unsigned i = 0; i < sizeof(dest); i++)
        {
            dest[i] = src[i];
        }

        dest[sizeof(dest)] = '\0';
    }

    constexpr unsigned msToMinute(long &&ms)
    {
        return ((ms / (1000 * 60)) % 60);
    }

    constexpr unsigned msToSecond(long &&ms)
    {
        return ((ms / 1000) % 60);
    }

    constexpr unsigned msToHours(long &&ms)
    {
        return ((ms / (1000 * 60 * 60)) % 24);
    }



} // end namespace rainbow

#endif // UTILS_HPP
