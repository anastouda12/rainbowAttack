#include "src/rainbowAttack.hpp"
#include "src/reduction.hpp"
#include "src/sha256.h"


namespace rainbow
{

    RainbowAttack::RainbowAttack(std::string &hashToCrackFilePath):
        rainbowTableFile_{},
        LoginAndHashToCrack_{},
        passwordCracked_{}
    {
        getHashesToCrack(hashToCrackFilePath);
    }

    void RainbowAttack::getHashesToCrack(const std::string &filepath)
    {
        std::ifstream in(filepath);

        if (in.fail())
        {
            std::cerr << "[ERROR] : " << "cannot open the file : " << filepath <<
                      std::endl;
            std::exit(-3);
        }

        // Read the login, hash pairs.
        std::string login, hash(SHA256_LENGTH, ' ');
        in >> login;
        in >> hash;

        while (in)
        {
            this->LoginAndHashToCrack_.push_back(make_pair(login, hash));
            in >> login;
            in >> hash;
        }

        in.close();
    }

    RTChain RainbowAttack::pwdBinarySearch(std::string &reduc)
    {
        RTChain chain;

        if (rainbowTableFile_.is_open())
        {
            rainbowTableFile_.clear();
            long min = rainbowTableFile_.seekg(0,
                                               std::ios::beg).tellg() / RTCHAIN_SIZE;
            long max = rainbowTableFile_.seekg(0,
                                               std::ios::end).tellg() / RTCHAIN_SIZE;
            long mid = (min + max) / 2;
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

                mid = (min + max) / 2;
            }
        }

        return chain;
    }

    std::string RainbowAttack::crackPassword(
            std::string &hash)
    {

        std::string password(PASSWORD_SIZE, ' ');
        std::string head(PASSWORD_SIZE, ' ');
        std::string previous(PASSWORD_SIZE, ' ');
        std::string current = head;
        unsigned step = HASH_LEN - 1; // last function reduction used
        unsigned nbFailures = 0;
        //rainbow::reduce(hash, current, step, 8);
        RTChain headTail = pwdBinarySearch(current);
        nbFailures++;

        while (headTail.head[0] == '\0' && nbFailures < HASH_LEN)
        {
            current = sha256(hash);

            for (unsigned i = nbFailures; i > 0; i--)
            {
                //rainbow::reduce(current, current, (HASH_LEN - 1) - i,
                //  8); // HASH_LEN - 1 its the last reduction used
                current = sha256(current);
            }

            headTail = pwdBinarySearch(current);
            nbFailures++;
        }

        if (headTail.head[0] != '\0') // head found ==> chain found
        {
            current = headTail.head[0];
            previous = current;
            bool found = false;
            int column = 0;

            while (!found &&
                            current !=
                            headTail.tail) // while we dont reach the tail of the chains
            {
                current = sha256(current);

                if (current == hash) // Hash of the password found
                {
                    found = true; // The previous reduce = password
                    password = previous;
                }
                else
                {
                    //rainbow::reduce(current, current, column, 8);
                    previous = current;
                    column++;
                }
            }
        }

        return password;
    }


    void RainbowAttack::attack()
    {
        rainbowTableFile_.open(FILE_NAME_RTABLE, std::ios::binary);

        if (rainbowTableFile_.fail())
        {
            std::cerr << "[ERROR] : " << "cannot open the file : " <<
                      FILE_NAME_RTABLE <<
                      std::endl;
            std::exit(-3);
        }

        rainbowTableFile_.seekg(0, std::ios::end);

        if (rainbowTableFile_.tellg() == 0)
        {
            std::cerr << "[ERROR] : " << "RainbowTable : " <<
                      FILE_NAME_RTABLE << " is empty, you need to generate the table first."
                      <<
                      std::endl;
            std::exit(-3);
        }

        rainbowTableFile_.clear();
        rainbowTableFile_.seekg(0, std::ios::beg);
        passwordCracked_.open("passCracked.txt", std::ios::binary);

        for (std::pair<std::string, std::string> lh :
                        this->LoginAndHashToCrack_)
        {
            passwordCracked_ << lh.first << ":" << crackPassword(lh.second) <<
                             std::endl; // Second = Hash  First = Login
        }

        rainbowTableFile_.close();
        passwordCracked_.close();
    }

    RainbowAttack::~RainbowAttack()
    {
        this->LoginAndHashToCrack_.clear();
    }





} // end namespace rainbow

