#ifndef REDUCTION_HPP
#define REDUCTION_HPP

#include "utils.hpp"

namespace rainbow
{

    /**
     * @brief hex2bin
     * Transform a text to array of bytes.
     * @param s text to transform.
     * @param data where the bytes array will be generate.
     * @return return size of the array of bytes.
     */
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

    /**
     * @brief REDUCE
     * Reduction function, reduce an hash to a plainPassword given in parameter.
     * @param hash hash to reduce.
     * @param red char array where to generate the plainPassword
     * @param step step of the reduction function inside the rainbowTable.
     * @param len password length, by default 8.
     */
    constexpr void REDUCE(const string &hash, char *red, unsigned step, unsigned len = PASSWORD_SIZE)
    {
        unsigned cur_in = 0;
        unsigned char bytes[SHA256_DIGEST_SIZE] = {0};
        hex2bin(hash.c_str(), bytes);

        for (unsigned i = 0; i < len; ++i)
        {
            cur_in = bytes[(i + step) & 31]; // SHA256_DIGEST_SIZE(32 = 2^5) - 1 = 31 , util for & bift shifting to avoid modulo
            red[i] = ALPHABET[cur_in % ALPHABET_SIZE];
        }
    }

} // end namespace rainbow



#endif // REDUCTION_HPP
