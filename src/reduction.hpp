#ifndef REDUCTION_HPP
#define REDUCTION_HPP

#include "utils.hpp"

namespace rainbow
{

    constexpr size_t hex2bin(const char *s, uint8_t *data)
    {
        size_t len = 0;
        char temp[] = {'0', '0', '\0'};

        while (*s && *(s + 1))
        {
            temp[0] = *s++;
            temp[1] = *s++;
            *data++ = strtoul(temp, NULL, 16);
            len++;
        }

        return len;
    }

    constexpr void REDUCE(const string &hash, char *red, unsigned col, unsigned len = PASSWORD_SIZE)
    {
        unsigned cur_in = 0;
        unsigned char bytes[SHA256_DIGEST_SIZE] = {0};
        hex2bin(hash.c_str(), bytes);

        for (unsigned i = 0; i < len; ++i)
        {
            cur_in = bytes[((i + col) & (SHA256_DIGEST_SIZE - 1))];
            red[i] = ALPHABET[cur_in % ALPHABET_SIZE];
        }
    }

} // end namespace rainbow



#endif // REDUCTION_HPP
