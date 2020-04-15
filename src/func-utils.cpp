#include "src/headers/func-utils.hpp"
#include <fstream>
#include <iomanip>
#include <iostream>



namespace rainbow {

void hexConvert(const char *text, unsigned char *bytes)
{
    unsigned int temp;
    int sha256_size = 32;
    for (int i = 0; i < sha256_size; i++) {
        std::sscanf(text + 2 * i, "%2x", &temp);
        bytes[i] = temp;
    }
}

std::vector<std::string> getHashesInit(std::string file)
{
    std::ifstream in(file);
    std::vector<std::string> hashes;
    if(in.fail())
    {
        std::cerr << "[ERROR]: " << "cannot open the file : " << file << std::endl;
        std::exit(-3);
    }
    std::string hash;
    while(std::getline(in, hash))
    {
        hashes.push_back(hash);
    }
    return hashes;
}

void writePrecomputedValuesIntoTable(std::ofstream & table, std::set<std::pair<std::string,std::string>> precomputedValues)
{
    table.clear();
    table.seekp(0, std::ios_base::beg);
   for(std::pair<std::string, std::string> headTailsChain : precomputedValues )
   {
       table << headTailsChain.first << " " << headTailsChain.second << std::endl;
   }
}

float getSizeOnBytes(const float mb)
{
    const unsigned byteToMb = 1048576;
    return mb * byteToMb;
}

}
