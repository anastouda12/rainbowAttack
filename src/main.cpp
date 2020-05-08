#include "src/rainbowTableGen.hpp"
#include "src/rainbowAttack.hpp"

using namespace std;
using namespace rainbow;

static void help()
{
    // Tell the user how to run the program
    std::cerr << "[USAGE]: " <<
              "./RainbowAttack <command> <option> :" <<
              std::endl;
    std::cerr << "<command...generate>" << "  " <<
              "<option...sizeTable in MB> " << std::endl;
    std::cerr << "<command...attack>" << "  " <<
              "<option...pathFile to hashes passwords>" << std::endl;
}

static int commandManager(int argc, char **argv)
{
    if (argc < 2)
    {
        help();
        return -1;
    }

    if (strcmp(argv[1], "generate") == 0)
    {
        if (argc < 3)
        {
            std::cerr << "[USAGE]: " <<
                      "Size of table in MB needed : <generate> <sizeTable in MB>" <<
                      std::endl;
            return -1;
        }

        try
        {
            float &&size_table = std::stof(argv[2]);
            rainbow::RainbowTableGen gen{std::move(size_table)};
            gen.generateTable();
        }
        catch (...)
        {
            std::cerr << "[ERROR]: " << "bad size used : " << argv[2] <<
                      std::endl;
            return -3;
        }
    }
    else if (strcmp(argv[1], "attack") == 0)
    {
        if (argc < 3)
        {
            std::cerr << "[USAGE]: " <<
                      "Path of file containing hashes passwords needed : <attack> <pathFile>"
                      << std::endl;
            return -3;
        }

        rainbow::RainbowAttack att{argv[2]};
        att.attack();
    }
    else
    {
        help();
        return -1;
    }

    return -2;
}


int main(int argc, char **argv)
{
    return commandManager(argc, argv);
}


