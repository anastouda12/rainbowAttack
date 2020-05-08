#include "src/rainbowAttack.hpp"
#include "src/reduction.hpp"
#include "src/sha256.h"


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

    RTChain RainbowAttack::pwdBinarySearch(std::string &reduc)
    {
        RTChain chain;
        rainbowTableFile_.open(FILE_NAME_RTABLE, std::ios::binary);

        if (rainbowTableFile_.is_open())
        {
            rainbowTableFile_.clear();
            long min = rainbowTableFile_.seekg(0,
                                               std::ios::beg).tellg() / RTCHAIN_SIZE;
            long max = rainbowTableFile_.seekg(0,
                                               std::ios::end).tellg() / RTCHAIN_SIZE;
            long mid =  mid = (min + max) >> 1;
            int comp;
            rainbowTableFile_.clear();
            rainbowTableFile_.seekg(0, std::ios::beg);

            while (min <= max)
            {
                rainbowTableFile_.seekg(mid * RTCHAIN_SIZE);
                rainbowTableFile_.read((char *) &chain, RTCHAIN_SIZE);
                comp = reduc.compare(chain.tail);

                if (comp == 0)
                {
                    return chain;
                }
                else if (comp < 0)
                {
                    max = mid - 1;
                }
                else
                {
                    min = mid + 1;
                }

                mid = (min + max) >> 1;
            }
        }

        rainbowTableFile_.close();
        return chain;
    }

    std::string RainbowAttack::crackPassword(
            std::string &hash)
    {
        bool foundChain = false;
        RTChain chain;
        std::string password(PASSWORD_SIZE, ' ');
        std::string current(PASSWORD_SIZE, ' ');
        unsigned attempt = 1;
        reduce(hash, chain.tail, HASH_LEN - attempt, PASSWORD_SIZE); // HASH_LEN -1 => Last reduction
        ++attempt;
        current = std::move(chain.tail);
        chain = pwdBinarySearch(current);
        foundChain = !isEmptyChain(chain);

        while (!foundChain && attempt < HASH_LEN)
        {
            for (unsigned step = 0; step < attempt; ++step)
            {
                reduce(sha256(current), chain.tail, HASH_LEN - (attempt - step), PASSWORD_SIZE);
                current = std::move(chain.tail);
            }

            chain = pwdBinarySearch(current);
            foundChain = !isEmptyChain(chain);
            ++attempt;
        }

        if (foundChain)
        {
            bool foundPwd = false;
            current = std::move(chain.head);
            std::string previous = current;
            attempt = 0;

            while (!foundPwd && attempt < HASH_LEN)
            {
                current = sha256(previous);

                if (current.compare(hash) == 0)
                {
                    foundPwd = true;
                    password = std::move(previous);

                }
                else
                {
                    reduce(current, chain.tail, attempt, PASSWORD_SIZE);
                    previous = std::move(chain.tail);
                    ++attempt;
                }

            }

        }

        return password;
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

