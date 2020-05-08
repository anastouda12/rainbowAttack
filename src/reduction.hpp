#ifndef REDUCTION_H
#define REDUCTION_H

#include "utils.hpp"

namespace rainbow
{

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
        for (unsigned i = 0, currentindex = 0; i < pwdLength; ++i)
        {
            currentindex = hash[((i + column) &
                                               (SHA256_LENGTH - 1))]; // bitwise & ; SHA256_LENGTH is a power of 2
            reduction[i] = ALPHABET[currentindex % ALPHABET_SIZE];
        }
    }

} // end namespace rainbow

#endif // REDUCTION_H
