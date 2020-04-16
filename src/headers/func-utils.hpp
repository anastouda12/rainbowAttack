#ifndef FUNCUTILS_HPP
#define FUNCUTILS_HPP

#include <iostream>
#include <fstream>
#include <vector>

#define msToMinute(ms) ((ms/(1000*60))%60)
#define msToHours(ms) ((ms/(1000*60*60))%24)
#define msToSecond(ms) ((ms/1000)%60)

namespace rainbow {

constexpr const char * ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                   "abcdefghijklmnopqrstuvwxyz"
                                   "0123456789";

void hexConvert(const char *text, unsigned char *bytes);

std::vector<std::string> getHashesInit(std::string file);

float getSizeOnBytes(const float mb);

} // end namespace rainbow

#endif // FUNCUTILS_HPP
