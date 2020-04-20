#include "src/headers/rainbowTableGen.hpp"
#include <iostream>
#include <string.h>
#include "src/headers/sha256.h"
#include "src/headers/reduction.hpp"
#include "src/headers/func-utils.hpp"
#include "src/headers/passwd-utils.hpp"
#include <chrono>

using namespace std::chrono;
namespace rainbow {

RainbowTableGen::RainbowTableGen(const float size):
size_{size}, tablePrecomputed_{}, rainbowTableFile_{}
{}

RainbowTableGen::RainbowTableGen():
    size_{0}, tablePrecomputed_{}, rainbowTableFile_{}
{}

RainbowTableGen::~RainbowTableGen()
{
    if(this->rainbowTableFile_.is_open()) this->rainbowTableFile_.close();
    this->tablePrecomputed_.clear();
}

void RainbowTableGen::generateTable()
{
    rainbowTableFile_.open("RainbowTable.txt", std::ios::out | std::ios::binary | std::ios::trunc);
    if(this->rainbowTableFile_.is_open()){
    std::cout << ">>>>>>>  RainbowTable generator has started.  <<<<<<<" << std::endl;
    std::cout << "======================================================" << std::endl;
    float sizeTableOnByte = getSizeOnBytes(this->size_);
    float currentSize = 0;
    std::cout  << std::fixed << std::setprecision(4);
    std::cout << "[INFO] : Generation table of size +- : " << sizeTableOnByte << " Bytes" << std::endl;
    auto start = high_resolution_clock::now();
    unsigned counterEnregistrement = 0;
    std::pair<std::string, std::string> precomputed;
    while(currentSize < sizeTableOnByte)
    {
        counterEnregistrement++;
        precomputed = buildPrecomputedHashChain();
        insertToPrecomputedOrdered(precomputed);
        currentSize += precomputed.first.length() + precomputed.second.length() + 1;
        std::cout  << std::fixed << std::setprecision(5) << std::setw(17);
        std::cout << (double) ((double) currentSize / (double) sizeTableOnByte)*100 << "% \r";
        std::cout.flush();
    }
    std::cout << "\r" << std::flush;
    writePrecomputedValuesIntoTable();
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    std::cout << "100%... Loading finish !" << std::endl;
    std::cout << "[SUCCES] : RainbowTable was generated on file 'Build/RainbowTable.txt'" <<std::endl;
    std::cout << "[DURATION] : Generated on " << std::fixed << std::setprecision(5);
    std::cout << msToHours(duration.count()) << " hours : ";
    std::cout << msToMinute(duration.count()) << " minutes : ";
    std::cout << msToSecond(duration.count()) << " seconds" << std::endl;
    std::cout << "[STATS] : " << counterEnregistrement << " passwords generated" << std::endl;
    std::cout << "======================================================" << std::endl;
    }else{
        std::cerr << "[ERROR] : Opening file 'RainbowTable.txt' failed" << std::endl;
    }
    this->rainbowTableFile_.close();
}

std::string RainbowTableGen::calculTail(std::string password) const
{
    Reduction reduction(password.length(),ALPHABET);
    for(unsigned column = 0;column <HASH_LEN; column++)
    {
        password = sha256(password);
        password = reduction.reduce(password,column);
    }
    return password;
}

std::pair<std::string,std::string> RainbowTableGen::buildPrecomputedHashChain() const
{
    std::pair<std::string, std::string> headTailsChain;
    std::string password = generate_passwd(random(MAXIMAL_PASSWORD_LENGTH,MAXIMAL_PASSWORD_LENGTH));
    std::string tail = calculTail(password);
    headTailsChain.first = password;
    headTailsChain.second = tail;
    return headTailsChain;
}

void RainbowTableGen::writePrecomputedValuesIntoTable()
{
    if(this->rainbowTableFile_.is_open()){
        this->rainbowTableFile_.clear();
        this->rainbowTableFile_.seekp(0, std::ios_base::beg);
        for(std::pair<std::string, std::string> headTailsChain : this->tablePrecomputed_ )
        {
           this->rainbowTableFile_ << headTailsChain.first << " " << headTailsChain.second << std::endl;
        }
    }else{
        std::cerr << "[ERROR] : Opening file 'RainbowTable.txt' failed" << std::endl;
    }

}

void RainbowTableGen::loadFromFile(const std::string &fileName)
{
    std::ifstream in(fileName.c_str(),std::ios::binary);
    if (in.is_open()) {
        // Read the chains.
        std::string pwd, hash;
        in >> pwd;
        in >> hash;
        while(in) { // While file contains rows, read and insert in the rainbowtable
            this->tablePrecomputed_.push_back(make_pair(pwd,hash));
            in >> pwd;
            in >> hash;
        }
    }else{
        std::cerr << "[ERROR] : rainbowTable not found  : \"" << fileName << "\"." << std::endl;
    }
    // we get the size of file
    in.close();
    std::ifstream ss(fileName, std::ifstream::ate | std::ifstream::binary); // we need ate flag
    this->size_ = getSizeOnMegaBytes(ss.tellg());
    in.close();
}

std::vector<std::pair<std::string,std::string>> RainbowTableGen::getTablePrecomputed() const
{
    return this->tablePrecomputed_;
}

void RainbowTableGen::insertToPrecomputedOrdered(const std::pair<std::string, std::string> & pair)
{
    // Find the position where the pair should be inserted, and insert it. O(log n) time.
   int index = findIndexOrdered(pair.second);
   this->tablePrecomputed_.insert(this->tablePrecomputed_.begin()+index, pair);

}

int RainbowTableGen::findIndexOrdered(const std::string & hash) const
{
    // Searches for hash using the binary search algorithm.
    int min = 0, max = this->tablePrecomputed_.size() - 1;
        int mid = (min + max) / 2, comp;
        while (min <= max) {
            comp = hash.compare(this->tablePrecomputed_[mid].second);
            if (comp == 0) {
                return mid;
            } else if (comp < 0) {
                max = mid - 1;
            } else {
                min = mid + 1;
            }
            mid = (min + max) / 2;
        }
        return min;
}


}
