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

    /**
     * @brief reduce Reduction function to given a reduce with a hash and step into the table
     * @param hash hash to reduce
     * @param column the column (step) of the reduction in the table
     * @return reduction of the hash given
     */
    constexpr void reduce(const std::string &hash,
                          char *reduction,
                          unsigned column, unsigned pwdLength)
    {
        int currentindex = 0;
        unsigned char bytes[SHA256_DIGEST_SIZE] = "";
        hex2bin(hash.c_str(), bytes);

        for (unsigned i = 0; i < pwdLength; ++i)
        {
            currentindex = bytes[((i + column) &
                                                (SHA256_DIGEST_SIZE - 1))]; // bitwise & ; SHA256_DIGEST_SIZE is a power of 2
            reduction[i] = ALPHABET[currentindex % ALPHABET_SIZE];
        }

        reduction[pwdLength] = '\0';

    }

} // end namespace rainbow



#endif // REDUCTION_HPP
