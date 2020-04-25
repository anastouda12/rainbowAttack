#ifndef REDUCTION_H
#define REDUCTION_H

#define SHA256_SIZE_BYTE 32

#include "utils.hpp"

namespace rainbow
{

class Reduction
{

    unsigned passwordLength_;
    std::string alphabet_;

  public:

    Reduction(unsigned passLength):
        passwordLength_{passLength}
    {}

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
