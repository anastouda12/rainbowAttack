#include <iostream>
#include "src/headers/rainbowTableGen.hpp"

using namespace std;

int main()
{
    rainbow::RainbowTableGen red(0.001,"anas.txt");
    red.generate();
    return 0;
}
