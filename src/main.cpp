#include <iostream>
#include "src/headers/rainbowTableGen.hpp"
#include "src/headers/rainbowAttack.hpp"

using namespace std;

int commandManager(int argc, char **argv);
void help();
void rainbowTableGenerate(float size);
void rainbowAttack(std::string pwdPath);

int main(int argc, char **argv)
{
    return commandManager(argc,argv);
}


int commandManager(int argc, char**argv)
{
    if (argc < 2) {
            help();
            return -1;
     }
    if(strcmp(argv[1], "generate") == 0){
        if(argc < 3){
            std::cerr << "[USAGE]: " << "Size of table in MB needed : <generate> <sizeTable in MB>" << std::endl;
            return -1;
        }
        try {
            float size = std::stof(argv[2]);
            rainbowTableGenerate(size);
        } catch (...) {
            std::cerr << "[ERROR]: " << "bad size used : " << argv[2] << std::endl;
            return -3;
        }
    }else if(strcmp(argv[1], "attack") == 0){
        if(argc < 3){
            std::cerr << "[USAGE]: " << "Path of file containing hashes passwords needed : <attack> <pathFile>" << std::endl;
        }
       rainbowAttack(argv[2]);
    }else{
        help();
        return -1;
    }
    return -2;
}

void help()
{
    // Tell the user how to run the program
        std::cerr << "[USAGE]: " <<
                  "./RainbowAttack <command> <option> :" <<
                  std::endl;
        std::cerr << "<command...generate>" << "  " << "<option...sizeTable in MB> " << std::endl;
        std::cerr << "<command...attack>" << "  " << "<option...pathFile to hashes passwords>" << std::endl;
}

void rainbowTableGenerate(float size)
{
    rainbow::RainbowTableGen gen(size);
    gen.generateTable();

}

void rainbowAttack(std::string pwdPath)
{
    rainbow::RainbowTableGen gen;
    gen.loadFromFile("RainbowTable.txt");
    rainbow::RainbowAttack att(gen,pwdPath);
}


