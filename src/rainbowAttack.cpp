#include "src/rainbowAttack.hpp"


namespace rainbow
{

    RainbowAttack::RainbowAttack(std::string &&hashToCrackFilePath):
        rainbowTableFile_{},
        passwordCracked_{},
        passwordHashes_{},
        INDEX_MIN_RT{getMinIndexTable()},
        INDEX_MAX_RT{getMaxIndexTable()}
    {

        passwordHashes_.open(hashToCrackFilePath, std::ios::in);
        rainbowTableFile_.open(FILE_NAME_RTABLE, std::ios::in | std::ios::binary);

        if (rainbowTableFile_.fail())
        {
            std::cerr << "[ERROR] : RainbowTable file : " << FILE_NAME_RTABLE << " failed to opening" << std::endl;
            exit(-3);
        }

        if (passwordHashes_.fail())
        {
            std::cerr << "[ERROR] : Password hashes file : " << hashToCrackFilePath << " failed to opening" << std::endl;
            exit(-3);
        }


    }



    std::vector<std::string> RainbowAttack::getHashesToCrack()
    {
        std::vector<std::string> vecHashes;
        vecHashes.reserve(100); // 100 passwords +-
        std::string hash(SHA256_BLOCKSIZE, ' ');

        while (std::getline(passwordHashes_, hash))
        {
            vecHashes.push_back(std::move(hash));
        }

        passwordHashes_.close();
        return vecHashes;
    }

    void RainbowAttack::attack()
    {

        passwordCracked_.open(PASSWORD_CRACKED_FILE, std::ios::binary);
        std::vector<std::string> &&hashes = getHashesToCrack();

        for (auto hash : hashes)
        {
            passwordCracked_ << crackPassword(hash) << std::endl;
        }

        passwordCracked_.close();
        rainbowTableFile_.close();
    }

    RainbowAttack::~RainbowAttack()
    {
    }





} // end namespace rainbow

