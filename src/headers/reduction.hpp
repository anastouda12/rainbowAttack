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
    Reduction(unsigned passLength, std::string alphabet):
        passwordLength_{passLength}, alphabet_{alphabet}
    {}

    inline std::string reduce(const std::string hash,unsigned column)
   {

       std::string reduction;
       unsigned char bytes[SHA256_SIZE_BYTE];
       int current;
       hexConvert(hash.c_str(),bytes);

       for(unsigned i =0;i< this->passwordLength_;i++)
       {
           current = bytes[(i + column) % SHA256_SIZE_BYTE];
           reduction += this->alphabet_[current % this->alphabet_.size()];
       }
       return reduction;
   }

    inline unsigned getPasswordLength()
    {
        return this->passwordLength_;
    }

    inline std::string getAlphabet()
    {
        return this->alphabet_;
    }

    inline void setPasswordLength(unsigned int length)
    {
        this->passwordLength_ = length;
    }

   inline void setAlphabet(std::string alphabet)
    {
        this->alphabet_ = alphabet;
    }

};

} // end namespace rainbow

#endif // REDUCTION_H
