#ifndef REDUCTION_H
#define REDUCTION_H

#define SHA256_SIZE_BYTE 32

#include <vector>
#include <stdio.h>
#include <string>
#include <math.h>
#include "func-utils.hpp"

namespace rainbow {

class Reduction
{

    unsigned passwordLength_;
    std::string alphabet_;

public:
    Reduction(unsigned passLength, std::string alphabet);
    std::string reduce(const std::string hash,unsigned column);
    void setPasswordLength(unsigned length);
    void setAlphabet(std::string alphabet);
};

} // end namespace rainbow

#endif // REDUCTION_H
