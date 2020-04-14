#include "src/headers/reduction.hpp"
#include "src/headers/func-utils.hpp"


namespace rainbow
{

Reduction::Reduction(unsigned passLength, std::string alphabet):
    passwordLength_{passLength}, alphabet_{alphabet}
{}

 std::string Reduction::reduce(const std::string hash,unsigned column)
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

 void Reduction::setPasswordLength(unsigned int length)
 {
     this->passwordLength_ = length;
 }

 void Reduction::setAlphabet(std::string alphabet)
 {
     this->alphabet_ = alphabet;
 }
}
