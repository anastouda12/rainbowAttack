#include "src/headers/rainbowAttack.hpp"
#include "src/headers/reduction.hpp"
#include "src/headers/sha256.h"
#include <iostream>


namespace rainbow {

RainbowAttack::RainbowAttack(RainbowTableGen & table): rainbowTable_{table}
{}

// source : Binary search inspired by https://www.geeksforgeeks.org/
std::optional<std::pair<std::string,std::string>> RainbowAttack::pwdBinarySearch(const std::string & hash) const
{
    std::vector<std::pair<std::string, std::string>> table = rainbowTable_.getTablePrecomputed();
    std::optional<std::pair<std::string,std::string>> headTail;
    int min = 0, max = table.size() - 1, mid = (min + max) / 2, comp;
        while (min <= max) {
            comp = hash.compare(table[mid].second);
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

    Reduction reduction(hash.size(),ALPHABET);
    std::optional<std::string> password;
    std::string head;
    std::string previous;
    std::string current = head;
    bool foundPassword = false;
    unsigned step =0;
    unsigned nbFailuresToFindHead = 0;
    std::optional<std::pair<std::string,std::string>> headTail = pwdBinarySearch(hash);
    while (nbFailuresToFindHead < 10 && step <= 50000 && !foundPassword) {
        if(headTail.has_value()){ // tail found ==> head found
            head = headTail.value().first;
            current = head;
            previous = current;
            current = sha256(current);
            if(pwdBinarySearch(current).has_value()){
               foundPassword = true;
               password.emplace(previous);
               return password;
            }else{
               current = reduction.reduce(current,step);
               step++;
            }
        }else{ // No tail found in table for find the head
            nbFailuresToFindHead++;
            current = reduction.reduce(hash,step);
            current = sha256(hash);
            headTail = pwdBinarySearch(hash);
        }
    }
    return password;
}





} // end namespace rainbow

