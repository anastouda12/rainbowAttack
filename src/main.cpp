#include <iostream>
#include "src/headers/rainbowTableGen.hpp"
#include "src/headers/rainbowAttack.hpp"

using namespace std;

int main()
{
    rainbow::RainbowTableGen red(400);
    red.generateTable();
    return 0;
}
