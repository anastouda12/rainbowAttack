#include "src/headers/rainbowTableGen.hpp"
#include <iostream>
#include <set>
#include "src/headers/sha256.h"
#include "src/headers/reduction.hpp"
#include "src/headers/passwd-utils.hpp"
#include "src/headers/func-utils.hpp"

namespace rainbow {

RainbowTableGen::RainbowTableGen(const float size, std::string path):
size_{size}, ofstreamRainbowTable_{path}
{
    ofstreamRainbowTable_.open(path, std::ios::out | std::ios::app);
}

RainbowTableGen::~RainbowTableGen()
{
    this->ofstreamRainbowTable_.close();
}

void RainbowTableGen::print()
{
    std::cout << ">  RainbowTable generator has started.  <" << std::endl;
    std::cout << "=========================================" << std::endl;

    std::cout << "[SUCCES] : RainbowTable was generated." << std::endl;
    std::cout << "========================================" << std::endl;

}

void RainbowTableGen::generate()
{
    std::set<std::pair<std::string,std::string>> tablePrecomputed;
    for(unsigned i = 0; i < this->size_ *1024 *1024;i+= sizeof (std::pair<std::string,std::string>))
    {
        std::pair<std::string, std::string> precomputed = buildPrecomputedHashChain();
        tablePrecomputed.insert(precomputed);
    }
    writePrecomputedValuesIntoTable(this->ofstreamRainbowTable_,tablePrecomputed);
}

std::string RainbowTableGen::getTail(std::string password) const
{
    Reduction reduction(password.length(),ALPHABET);
    for(unsigned column = 0;column < HASH_LEN; column++)
    {
        password = sha256(password);
        password = reduction.reduce(password,column);
    }
    return password;
}

std::pair<std::string,std::string> RainbowTableGen::buildPrecomputedHashChain() const
{
    std::pair<std::string, std::string> headTailsChain;
    std::string password = generate_passwd(random(MINIMAL_PASSWORD_LENGTH,MAXIMAL_PASSWORD_LENGTH));
    std::string tail = getTail(password);
    headTailsChain.first = password;
    headTailsChain.second = tail;
    return headTailsChain;
}


}
