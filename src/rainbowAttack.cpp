#include "src/headers/rainbowAttack.hpp"
#include "src/headers/reduction.hpp"
#include "src/headers/sha256.h"
#include <iostream>


namespace rainbow {

RainbowAttack::RainbowAttack(RainbowTableGen & table, std::string pathPasswordsToCrack): rainbowTable_{table}, LoginAndHashToCrack_{}
{
    initPasswordsToCrack(pathPasswordsToCrack);
}

// source : Binary search inspired by https://www.geeksforgeeks.org/
std::optional<std::pair<std::string,std::string>> RainbowAttack::pwdBinarySearch(const std::string & pwd) const
{
    std::vector<std::pair<std::string, std::string>> table = rainbowTable_.getTablePrecomputed();
    std::optional<std::pair<std::string,std::string>> headTail;
    int min = 0, max = table.size() - 1, mid = (min + max) / 2, comp;
        while (min <= max) {
            comp = pwd.compare(table[mid].second);
            if (comp == 0) {
                headTail.emplace(table[mid]);
                return headTail;
            }else if(comp < 0){
                max = mid - 1;
            }else{
                min = mid + 1;
            }
            mid = (min + max) / 2;
        }
        return headTail;
}

std::optional<std::string> RainbowAttack::crackPassword(const std::string & hash) const
{

    Reduction reduction(8,ALPHABET);
    std::optional<std::string> password;
    std::string head;
    std::string previous;
    std::string current = head;
    unsigned step =HASH_LEN-1; // last function reduction used
    unsigned nbFailures =0;
    std::cout << hash << std::endl;
    current = reduction.reduce(hash,step);
    std::optional<std::pair<std::string,std::string>> headTail = pwdBinarySearch(current);
    nbFailures++;
    while(!headTail.has_value() && nbFailures < HASH_LEN){
        current = sha256(hash);
        for(unsigned i = 0; i <= nbFailures; i++){
            current = reduction.reduce(current,(HASH_LEN-1)-i); // HASH_LEN - 1 its the last reduction used
            current = sha256(current);
        }
        headTail = pwdBinarySearch(current);
        nbFailures++;
        std::cout << nbFailures << std::endl;
    }
    if(headTail.has_value()) password = "ok";
    return password;
}


void RainbowAttack::initPasswordsToCrack(const std::string file)
{
    std::ifstream in(file);
    if(in.fail())
    {
        std::cerr << "[ERROR] : " << "cannot open the file : " << file << std::endl;
        std::exit(-3);
    }
    // Read the login, hash pairs.
    std::string login, hash;
    in >> login;
    in >> hash;
    while(in) { // While file contains rows, read and insert in the vector
        this->LoginAndHashToCrack_.push_back(make_pair(login,hash));
        in >> login;
        in >> hash;
    }
   in.close();
}

void RainbowAttack::attack()
{

    crackPassword(this->LoginAndHashToCrack_.at(0).second);
   // for(auto e : this->LoginAndHashToCrack_){
   //     std::cout << crackPassword(e.second).has_value() << std::endl;
    //}
}

RainbowAttack::~RainbowAttack(){
    this->LoginAndHashToCrack_.clear();
}





} // end namespace rainbow

