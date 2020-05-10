#ifndef RAINBOWATTACK_HPP
#define RAINBOWATTACK_HPP

#include <fstream>
#include <vector>
#include "src/utils.hpp"
#include "src/sha256.h"
#include "src/reduction.hpp"


#define searchChain(hash, chain)\
    std::string current(PASSWORD_SIZE, ' ');\
    for (unsigned i = HASH_LEN - 1; i > 0; --i){\
        current = hash;\
        for (unsigned j = i; j < HASH_LEN; ++j){\
            reduce(current, chain.tail, j, PASSWORD_SIZE);\
            current = sha256(chain.tail);\
        }\
        pwdBinarySearch(chain.tail,chain);\
        if (!isEmptyChain(chain)){\
            break;\
        }\
    }\

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
             * @brief LoginAndHashToCrack_ All pair login-hash to crack inside 'HashToCrackFile' given.
             */
            std::vector<std::string> hashesToCrack_;

            /**
             * @brief passwordCracked_ File containing the passwords cracked
             */
            std::ofstream passwordCracked_;



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
                return buildUpPassword(hash, chain);
            }


            /**
             * @brief pwdBinarySearch Binary search inside the rainbowtable to retrieves the chain who has the tail reduce given in param
             * @param reduc the reduction given to checks in the rainbowtable
             * @param chain the chain where the result chain will be - If found, if not found the chain will be empty
             */
            constexpr void pwdBinarySearch(std::string &&reduc, RTChain &chain)
            {
                bool found = false;
                rainbowTableFile_.open(FILE_NAME_RTABLE, std::ios::binary);

                if (rainbowTableFile_.is_open())
                {
                    rainbowTableFile_.clear();
                    long min = rainbowTableFile_.seekg(0,
                                                       std::ios::beg).tellg() / RTCHAIN_SIZE;
                    long max = rainbowTableFile_.seekg(0,
                                                       std::ios::end).tellg() / RTCHAIN_SIZE;
                    long mid =  mid = (min + max) >> 1;
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

                rainbowTableFile_.close();
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
            void getHashesToCrack(const std::string &filepath);


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
