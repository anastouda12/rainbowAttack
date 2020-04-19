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

float getSizeOnBytes(const float mb);

float getSizeOnMegaBytes(const float bytes);

} // end namespace rainbow

#endif // FUNCUTILS_HPP
