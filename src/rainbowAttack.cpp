#include "src/headers/rainbowAttack.hpp"
#include "src/headers/reduction.hpp"
#include "src/headers/sha256.h"
#include "src/headers/utils.hpp"
#include <iostream>


namespace rainbow
{

RainbowAttack::RainbowAttack(std::string & hashToCrackFilePath):
    rainbowTableFile_{},
    LoginAndHashToCrack_{},
    passwordCracked_{}
{
    getHashesToCrack(hashToCrackFilePath);
}

void RainbowAttack::getHashesToCrack(const std::string & filepath)
{
    std::ifstream in(filepath);
    if (in.fail())
    {
        std::cerr << "[ERROR] : " << "cannot open the file : " << filepath <<
                  std::endl;
        std::exit(-3);
    }
    // Read the login, hash pairs.
    std::string login, hash(SHA256_LENGTH, ' ');
    in >> login;
    in >> hash;
    while (in)  // While file contains rows, read and insert in the vector
    {
        this->LoginAndHashToCrack_.push_back(make_pair(login, hash));
        in >> login;
        in >> hash;
    }
    in.close();
}

// source : Binary search inspired by https://www.geeksforgeeks.org/
std::optional<std::pair<std::string, std::string>>
        RainbowAttack::pwdBinarySearch(std::string & pwd) const
{
    std::vector<std::pair<std::string, std::string>> table;
    std::optional<std::pair<std::string, std::string>> headTail;
    int min = 0, max = table.size() - 1, mid = (min + max) / 2, comp;
    while (min <= max)
    {
        comp = pwd.compare(table[mid].second);
        if (comp == 0)
        {
            headTail.emplace(table[mid]);
            return headTail;
        }
        else if (comp < 0)
        {
            max = mid - 1;
        }
        else
        {
            min = mid + 1;
        }
        mid = (min + max) / 2;
    }
    return headTail;
}

std::optional<std::string> RainbowAttack::crackPassword(
    std::string & hash) const
{

    Reduction reduction(MAXIMAL_PASSWORD_LENGTH);
    std::optional<std::string> password;
    std::string head;
    std::string previous;
    std::string current = head;
    unsigned step = HASH_LEN - 1; // last function reduction used
    unsigned nbFailures = 0;
    current = *reduction.reduce(hash, step);
    std::optional<std::pair<std::string, std::string>> headTail =
                pwdBinarySearch(current);
    nbFailures++;
    while (!headTail.has_value() && nbFailures < HASH_LEN)
    {
        current = sha256(hash);
        for (unsigned i = nbFailures; i > 0; i--)
        {
            current = *reduction.reduce(current,
                                        (HASH_LEN - 1) - i); // HASH_LEN - 1 its the last reduction used
            current = sha256(current);
        }
        headTail = pwdBinarySearch(current);
        nbFailures++;
        std::cout  << std::fixed << std::setprecision(5) << std::setw(17);
        std::cout << (double) ((double) nbFailures / (double)(
                                   HASH_LEN - 1.0)) * 100 << "% \r";
        std::cout.flush();
    }
    if (headTail.has_value()) password = "ok";
    return password;
}


void RainbowAttack::attack()
{
    rainbowTableFile_.open(FILE_NAME_RTABLE, std::ios::binary);
    if (rainbowTableFile_.fail())
    {
        std::cerr << "[ERROR] : " << "cannot open the file : " <<
                  FILE_NAME_RTABLE <<
                  std::endl;
        std::exit(-3);
    }
    rainbowTableFile_.seekg(0, std::ios::end);
    if (rainbowTableFile_.tellg() == 0)
    {
        std::cerr << "[ERROR] : " << "RainbowTable : " <<
                  FILE_NAME_RTABLE << " is empty, you need to generate the table first."
                  <<
                  std::endl;
        std::exit(-3);
    }
    rainbowTableFile_.clear();
    rainbowTableFile_.seekg(0, std::ios::beg);

    crackPassword(this->LoginAndHashToCrack_.at(0).second);
    // for(auto e : this->LoginAndHashToCrack_){
    //     std::cout << crackPassword(e.second).has_value() << std::endl;
    //}
}

RainbowAttack::~RainbowAttack()
{
    this->LoginAndHashToCrack_.clear();
}





} // end namespace rainbow

