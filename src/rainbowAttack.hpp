#ifndef RAINBOWATTACK_HPP
#define RAINBOWATTACK_HPP

#include <fstream>
#include <vector>
#include "src/utils.hpp"

namespace rainbow
{

    /**
     * @brief The RainbowAttack class
     * Used to attempt an RainbowAttack with SHA256 algorithm.
     */
    class RainbowAttack
    {

        private :

            /**
             * @brief File containing the rainbow table.
             */
            ifstream rainbowTableFile_;

            /**
             * @brief passwordCracked_ File containing the passwords cracked
             */
            ofstream passwordCracked_;

            /**
             * @brief File containing the hashes.
             */
            ifstream passwordHashes_;

            /**
             * @brief INDEX_MIN_RT
             * Index min of the rainbowTable
             */
            const int INDEX_MIN_RT;

            /**
             * @brief INDEX_MAX_RT
             * Index max of the rainbowTable
             */
            const int INDEX_MAX_RT;


            /**
             * @brief crackPassword
             * Crack a password of a hash given.
             * @param hash the hash of the password to crack.
             * @param rtable the rainbowtable , we use here a different ifstream for each threads to have different handler for
             * each of them.
             * @return the password cracked or "NOTFOUND" if its fail
             */
            string crackPassword(const string &hash, ifstream &rtable);

            /**
             * @brief crackSomePasswords
             * Crack some passwords inside a vector [work of a thread].
             * @param vec vector containing the hashes to crack
             */
            void crackSomePasswords(vector<string> &vec);


            /**
             * @brief pwdBinarySearch Binary search inside the rainbowtable to retrieves the chain who has the tail reduce given in param
             * @param reduc the reduction given to checks in the rainbowtable
             * @param chain the chain where the result chain will be - If found, if not found the chain will be empty
             * @param min Mini index of RT
             * @param max Max index of RT
             * @return return true if the entry is found.
             */
            bool pwdBinarySearch(const char *tail, rtEntry &chain, ifstream &rtable);

            /**
             * @brief buildUpPassword
             * Build up the password with the hash and the chain possibly containing the hash.
             * @param hash the hash of the password
             * @param chain the chain possibly containing the hash
             * @param password variable where to put the password.
             * @return true if the build up was successful
             */
            bool buildUpPassword(const string &hash, const rtEntry &chain, string &password);

            /**
             * @brief getHashesToCrack
             * Retrieves hashes to crack in x vector, where x is number of thread
             */
            vector<vector<string>> getHashesToCrack();

            /**
             * @brief getMinIndexTable
             * Get the first index of the RainbowTable
             * @return first index.
             */
            constexpr int getMinIndexTable()
            {
                int min = -1;

                if (!rainbowTableFile_.is_open())
                {
                    rainbowTableFile_.open(FILE_NAME_RTABLE, ios::in | ios::binary);
                }

                if (!rainbowTableFile_.fail())
                {
                    min = rainbowTableFile_.seekg(0, ios::beg).tellg() / RTENTRY_SIZE;
                    rainbowTableFile_.clear();
                    rainbowTableFile_.close();
                }
                else
                {
                    cerr << "[ERROR] : RainbowTable file : " << FILE_NAME_RTABLE << " failed to opening" << endl;
                    exit(EXIT_FAILURE);
                }

                return min;
            }

            /**
             * @brief getMaxIndexTable
             * Get the last index of the RainbowTable
             * @return last index.
             */
            constexpr int getMaxIndexTable()
            {
                int max = -1;

                if (!rainbowTableFile_.is_open())
                {
                    rainbowTableFile_.open(FILE_NAME_RTABLE, ios::in | ios::binary);
                }

                if (!rainbowTableFile_.fail())
                {
                    max = rainbowTableFile_.seekg(0, ios::end).tellg() / RTENTRY_SIZE;
                    rainbowTableFile_.clear();
                    rainbowTableFile_.close();
                }
                else
                {
                    cerr << "[ERROR] : RainbowTable file : " << FILE_NAME_RTABLE << " failed to opening" << endl;
                    exit(EXIT_FAILURE);
                }

                return max;
            }

        public:

            /**
             * @brief RainbowAttack Constructor of the rainbowAttack
             * @param hashToCrackFile file containing the hashes to crack
             */
            RainbowAttack(const string &hashToCrackFile);

            /**
              *@brief RainbowAttack destructor.
              */
            ~RainbowAttack();

            /**
             * @brief attack Attempt a rainbowAttack
             */
            void attack();



    };

} // end namespace rainbow

#endif // RAINBOWATTACK_HPP
