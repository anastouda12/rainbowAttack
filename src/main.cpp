#include "src/rainbowTableGen.hpp"
#include "src/rainbowAttack.hpp"

using namespace std;
using namespace rainbow;

/**
 * @brief help
 * Print usage
 */
static void help()
{
    cerr << "[USAGE]: " << "./RainbowTable command option, where :" << endl;
    cerr << "- command 'generate' to generate a rainbow table" << endl;
    cerr << "- command 'attack' to execute a rainbow attack" << endl;
    cerr << "- option size of table in MB with command 'generate'" << endl;
    cerr << "- option path to file containing hashes password with command 'attack'" << endl;
}

/**
 * @brief commandManager
 * Manages the command of the user.
 * @param argc argc
 * @param argv argv
 * @return exit code.
 */
static int commandManager(int argc, char **argv)
{
    if (argc < 2)
    {
        help();
        return EXIT_FAILURE;
    }

    if (strcmp(argv[1], "generate") == 0)
    {
        if (argc < 3)
        {
            cerr << "[USAGE]: " <<
                 "Size of table in MB needed : <generate> <sizeTable in MB>" <<
                 endl;
            return EXIT_FAILURE;
        }

        try
        {
            RainbowTableGen gen{stof(argv[2])};
            gen.generateTable();
            return EXIT_SUCCESS;
        }
        catch (...)
        {
            cerr << "[ERROR]: " << "bad size used : " << argv[2] <<
                 endl;
            return EXIT_FAILURE;
        }
    }
    else if (strcmp(argv[1], "attack") == 0)
    {
        if (argc < 3)
        {
            cerr << "[USAGE]: " <<
                 "Path of file containing hashes passwords needed : <attack> <pathFile>"
                 << endl;
            return EXIT_FAILURE;
        }

        RainbowAttack att{argv[2]};
        att.attack();
        return EXIT_SUCCESS;
    }
    else
    {
        help();
        return EXIT_FAILURE;
    }

    return EXIT_FAILURE;
}


int main(int argc, char **argv)
{
    return commandManager(argc, argv);
}


