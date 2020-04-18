#include <iostream>
#include "src/headers/rainbowTableGen.hpp"
#include "src/headers/rainbowAttack.hpp"

using namespace std;

int main()
{
    rainbow::RainbowTableGen red(0.0011);
    red.loadFromFile("RainbowTable.txt");
    rainbow::RainbowAttack at(red);
    return 0;
}
