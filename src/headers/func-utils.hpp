#ifndef FUNCUTILS_HPP
#define FUNCUTILS_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <set>

namespace rainbow {

constexpr const char * ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                   "abcdefghijklmnopqrstuvwxyz"
                                   "0123456789";

void hexConvert(const char *text, unsigned char *bytes);

std::vector<std::string> getHashesInit(std::string file);

void writePrecomputedValuesIntoTable(std::ofstream & table, std::set<std::pair<std::string,std::string>> precomputedValues);

} // end namespace rainbow

#endif // FUNCUTILS_HPP
