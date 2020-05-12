#ifndef RAINBOWATTACK_HPP
#define RAINBOWATTACK_HPP

#include <fstream>
#include <vector>
#include "src/utils.hpp"
#include "src/sha256.h"
#include "src/reduction.hpp"


namespace rainbow
{

    class RainbowAttack
    {

        private :

            /**
             * @brief File containing the rainbow table.
             */
            std::ifstream rainbowTableFile_;

            /**
             * @brief passwordCracked_ File containing the passwords cracked
             */
            std::ofstream passwordCracked_;

            /**
             * @brief File containing the hashes.
             */
            std::ifstream passwordHashes_;

            const int INDEX_MIN_RT;
            const int INDEX_MAX_RT;


            /**
             * @brief crackPassword Crack hash of a password
             * @param hash hash to retrieve password
             * @return password cracked if not empty string
             */
            inline std::string crackPassword(
                    std::string &hash)
            {
                RTChain chain;
                searchChain(hash, chain);

                if (!isEmptyChain(chain))
                {
                    return buildUpPassword(hash, chain);
                }

                return "NOTFOUNDCHAIN";
            }


            constexpr void searchChain(std::string &hash, RTChain &chain)
            {
                char current [SHA256_BLOCKSIZE] = "";

                for (unsigned i = HASH_LEN - 1; i > 0; --i)
                {
                    copyArrays(hash.c_str(), current, SHA256_BLOCKSIZE);

                    for (unsigned j = i; j < HASH_LEN; ++j)
                    {
                        reduce(current, chain.tail, j, PASSWORD_SIZE);
                        copyArrays(sha256(chain.tail).c_str(), current, SHA256_BLOCKSIZE);
                    }

                    pwdBinarySearch(chain.tail, chain, INDEX_MIN_RT, INDEX_MAX_RT);

                    if (!isEmptyChain(chain))
                    {
                        break;
                    }
                }
            }


            /**
             * @brief pwdBinarySearch Binary search inside the rainbowtable to retrieves the chain who has the tail reduce given in param
             * @param reduc the reduction given to checks in the rainbowtable
             * @param chain the chain where the result chain will be - If found, if not found the chain will be empty
             * @param min Mini index of RT
             * @param max Max index of RT
             */
            constexpr void pwdBinarySearch(std::string &&reduc, RTChain &chain, int min, int max)
            {
                if (rainbowTableFile_.is_open())
                {
                    bool found = false;
                    int mid =  mid = (min + max) >> 1;
                    int comp = 0;
                    rainbowTableFile_.clear();
                    rainbowTableFile_.seekg(0, std::ios::beg);

                    while (min <= max)
                    {
                        rainbowTableFile_.seekg(mid * RTCHAIN_SIZE);
                        rainbowTableFile_.read((char *) &chain, RTCHAIN_SIZE);
                        comp = reduc.compare(chain.tail);

                        if (comp == 0)
                        {
                            found = true;
                            break;
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

                    if (!found)
                    {
                        chain = RTChain();        // Chain not found - reset
                    }
                }
            }


            constexpr int getMinIndexTable()
            {
                int min = -1;

                if (!rainbowTableFile_.is_open())
                {
                    rainbowTableFile_.open(FILE_NAME_RTABLE, std::ios::in | std::ios::binary);
                }

                if (!rainbowTableFile_.fail())
                {
                    min = rainbowTableFile_.seekg(0, std::ios::beg).tellg() / RTCHAIN_SIZE;
                    rainbowTableFile_.clear();
                    rainbowTableFile_.close();
                }
                else
                {
                    std::cerr << "[ERROR] : RainbowTable file : " << FILE_NAME_RTABLE << " failed to opening" << std::endl;
                    exit(-3);
                }

                return min;
            }

            constexpr int getMaxIndexTable()
            {
                int max = -1;

                if (!rainbowTableFile_.is_open())
                {
                    rainbowTableFile_.open(FILE_NAME_RTABLE, std::ios::in | std::ios::binary);
                }

                if (!rainbowTableFile_.fail())
                {
                    max = rainbowTableFile_.seekg(0, std::ios::end).tellg() / RTCHAIN_SIZE;
                    rainbowTableFile_.clear();
                    rainbowTableFile_.close();
                }
                else
                {
                    std::cerr << "[ERROR] : RainbowTable file : " << FILE_NAME_RTABLE << " failed to opening" << std::endl;
                    exit(-3);
                }

                return max;
            }


            std::string buildUpPassword(std::string &hash, RTChain &chain)
            {
                std::string previous;
                std::string current;
                char reduction[PASSWORD_SIZE + 1];
                unsigned step{0};
                bool found{false};
                current = chain.head;

                while (step < HASH_LEN && !found)
                {
                    previous = current;
                    current = sha256(current);
                    found = current.compare(hash) == 0;
                    reduce(current, reduction, step, PASSWORD_SIZE);
                    current = reduction;
                    ++step;
                }

                if (!found)
                {
                    return "NOTFOUND";
                }

                return previous;

            }

            /**
             * @brief getHashesToCrack Retrieves the login hash pair to crack
             * @param filepath filepath to the file full of hashes
             */
            std::vector<std::string> getHashesToCrack();


        public:


            /**
             * @brief RainbowAttack Constructor of the rainbowAttack
             * @param hashToCrackFile file containing the hashes to crack
             */
            RainbowAttack(std::string &&hashToCrackFile);
            ~RainbowAttack();

            /**
             * @brief attack Attempt a rainbowAttack
             */
            void attack();



    };

} // end namespace rainbow

#endif // RAINBOWATTACK_HPP
