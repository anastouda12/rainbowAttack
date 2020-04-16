#include "src/headers/rainbowTableGen.hpp"
#include <iostream>
#include <set>
#include <string.h>
#include "src/headers/sha256.h"
#include "src/headers/reduction.hpp"
#include "src/headers/passwd-utils.hpp"
#include "src/headers/func-utils.hpp"

namespace rainbow {

RainbowTableGen::RainbowTableGen(const float size):
size_{size}, rainbowTableFile_{"RainbowTable.txt"}
{
    rainbowTableFile_.open("RainbowTable.txt", std::ios::out | std::ios::app);
}

RainbowTableGen::~RainbowTableGen()
{
    this->rainbowTableFile_.close();
}

void RainbowTableGen::generateTable()
{
    std::cout << ">>>>>>>  RainbowTable generator has started.  <<<<<<<" << std::endl;
    std::cout << "======================================================" << std::endl;
    std::set<std::pair<std::string,std::string>> tablePrecomputed;
    float sizeTableOnByte = getSizeOnBytes(this->size_);
    float currentSize = 0;
    std::cout  << std::fixed << std::setprecision(4);
    std::cout << "[INFO] : Generation table of size +- :" << sizeTableOnByte << " Bytes" << std::endl;
    while(currentSize < sizeTableOnByte)
    {
        std::pair<std::string, std::string> precomputed = buildPrecomputedHashChain();
        tablePrecomputed.insert(precomputed);
        currentSize += precomputed.first.size() + std::string(" ").size() + precomputed.second.size();
        std::cout  << std::fixed << std::setprecision(5) << std::setw(17);
        std::cout << (double) ((double) currentSize / (double) sizeTableOnByte)*100 << "% \r";
        std::cout.flush();
    }
    std::cout << "\r" << std::flush;
    writePrecomputedValuesIntoTable(this->rainbowTableFile_,tablePrecomputed);
    std::cout << "100% Finish !" << std::endl;
    std::cout << "[SUCCES] : RainbowTable was generated." << std::endl;
    std::cout << "======================================================" << std::endl;
}

std::string RainbowTableGen::calculTail(std::string password) const
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
    std::string tail = calculTail(password);
    headTailsChain.first = password;
    headTailsChain.second = tail;
    return headTailsChain;
}


}
