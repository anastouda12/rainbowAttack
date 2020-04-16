#include <iostream>
#include "src/headers/rainbowTableGen.hpp"

using namespace std;

int main()
{
    rainbow::RainbowTableGen red(0.0010);
    red.generateTable();
    return 0;
}
