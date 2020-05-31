#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <fstream>
#include <thread>

using namespace std;
using namespace std::chrono;
namespace rainbow
{

    /**
    * Configuration
    *
    */

    constexpr const char *ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                     "abcdefghijklmnopqrstuvwxyz"
                                     "0123456789";

    constexpr const char *FILE_NAME_RTABLE = "RainbowTable.txt";
    constexpr const char *PASSWORD_CRACKED_FILE = "PasswordsCracked.txt";

    static const unsigned num_cpus = thread::hardware_concurrency(); // number optimal thread
    constexpr const unsigned PASSWORD_SIZE = 8;
    constexpr const int HASH_LEN = 5000;
    constexpr const unsigned SHA256_DIGEST_SIZE = 32;
    constexpr const unsigned SHA256_BLOCKSIZE = 64;
    constexpr int ALPHABET_SIZE = 62;

    /**
     * @brief The rtChain struct Chain of a rainbowTable with the head and the tail
     */
    struct rtEntry
    {
        char head[PASSWORD_SIZE];
        char tail[PASSWORD_SIZE];
    };

    inline bool operator<(const rtEntry lhs,
                          const rtEntry rhs)
    {
        return strncmp(lhs.tail, rhs.tail, PASSWORD_SIZE) < 0;
    }

    constexpr const unsigned RTENTRY_SIZE = sizeof(rtEntry);

    constexpr bool isEmptyChain(const rtEntry chain)
    {
        return chain.head[0] == '\0' && chain.tail[0] == '\0';
    }

    inline bool is_emptyFile(std::ifstream &pFile)
    {
        return pFile.peek() == std::ifstream::traits_type::eof();
    }

    constexpr float getSizeOnBytes(float mb)
    {
        return mb * 1000000;
    }

    constexpr float getSizeOnMegaBytes(float bytes)
    {
        return bytes /  1000000;
    }


    constexpr void copyArrays(const char *src, char *dest, size_t size_dest)
    {
        for (size_t i = 0; i < size_dest; i++)
        {
            dest[i] = src[i];
        }

    }

    constexpr unsigned msToMinute(long ms)
    {
        return ((ms / (1000 * 60)) % 60);
    }

    constexpr unsigned msToSecond(long ms)
    {
        return ((ms / 1000) % 60);
    }

    constexpr unsigned msToHours(long ms)
    {
        return ((ms / (1000 * 60 * 60)) % 24);
    }

    inline void printDuration(milliseconds ms)
    {
        cout << "[DURATION] : Completed on ";
        cout << msToHours(ms.count()) << " hours : ";
        cout << msToMinute(ms.count()) << " minutes : ";
        cout << msToSecond(ms.count()) << " seconds" << endl;
    }


} // end namespace rainbow

#endif // UTILS_HPP
