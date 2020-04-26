#ifndef REDUCTION_H
#define REDUCTION_H

#define SHA256_SIZE_BYTE 32
#define SHA256_LENGTH 64

#include "utils.hpp"

namespace rainbow
{

class Reduction
{

    /**
     * @brief passwordLength_ pwd length
     */
    unsigned passwordLength_;

    /**
     * @brief alphabet_ Alphabet
     */
    std::string alphabet_;

  public:

    /**
     * @brief Reduction Constructor
     * @param passLength pass length
     */
    Reduction(unsigned passLength):
        passwordLength_{passLength}
    {}

    /**
     * @brief reduce Reduction function to given a reduce with a hash and step into the table
     * @param hash hash to reduce
     * @param column the column (step) of the reduction in the table
     * @return reduction of the hash given
     */
    inline std::string * reduce(std::string & hash,
                                unsigned column) const
    {
        static std::string reduction(this->passwordLength_, ' ');
        unsigned char bytes[SHA256_SIZE_BYTE];
        int current;
        hexConvert(hash.c_str(), bytes);

        for (unsigned i = 0; i < this->passwordLength_; i++)
        {
            current = bytes[(i + column) % SHA256_SIZE_BYTE];
            reduction[i] = ALPHABET[current % ALPHABET_SIZE];
        }
        return &reduction;
    }
};

} // end namespace rainbow

#endif // REDUCTION_H
