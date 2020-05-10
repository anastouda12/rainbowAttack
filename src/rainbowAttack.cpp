#include "src/rainbowAttack.hpp"


namespace rainbow
{

    RainbowAttack::RainbowAttack(std::string &&hashToCrackFilePath):
        rainbowTableFile_{},
        hashesToCrack_{},
        passwordCracked_{}
    {
        getHashesToCrack(hashToCrackFilePath);
        hashToCrackFilePath.reserve(100);
    }

    void RainbowAttack::getHashesToCrack(const std::string &filepath)
    {
        std::ifstream in(filepath);

        if (in.fail())
        {
            std::cerr << "[ERROR] : " << "cannot open the file : " << filepath <<
                      std::endl;
            std::exit(-6);
        }

        std::string hash(SHA256_LENGTH, ' ');

        while (std::getline(in, hash))
        {
            hashesToCrack_.push_back(std::move(hash));
        }

        in.close();
    }

    void RainbowAttack::attack()
    {

        passwordCracked_.open("passCracked.txt", std::ios::binary);

        for (std::string hash : this->hashesToCrack_)
        {
            passwordCracked_ << crackPassword(hash) << std::endl;
        }

        passwordCracked_.close();
    }

    RainbowAttack::~RainbowAttack()
    {
        this->hashesToCrack_.clear();

    }





} // end namespace rainbow

