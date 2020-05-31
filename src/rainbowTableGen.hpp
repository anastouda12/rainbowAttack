#ifndef RAINBOWTABLEGEN_HPP
#define RAINBOWTABLEGEN_HPP

#include <set>
#include "src/utils.hpp"
#include "src/sha256.h"
#include "src/reduction.hpp"
#include "passwd-utils.hpp"

namespace rainbow
{
    /**
     * @brief The RainbowTableGen class
     * Used to generate a RainbowTable with SHA256 algorithm.
     */
    class RainbowTableGen
    {

            /**
             * @brief Size on MegaByte of the rainbowTable.
             */
            float size_;

            /**
             * @brief File containing the rainbow table.
             */
            ofstream rainbowTableFile_;


            /**
             * @brief generateMiniTable
             * Generate a miniTable rainbow [work of a thread]
             * @param fileName fileName where to generate the rainbowTable.
             * different files for different thread.
             * @param nbchain_by_cpu number of chain to generate.
             */
            void generateMiniTable(const string &fileName, unsigned nbchain_by_cpu);


            /**
             * @brief buildPrecomputedHashChain
             * Build a precomputed hashChain.
             * @param chain the chain generated.
             */
            inline void buildPrecomputedHashChain(rtEntry &chain)
            {
                string password = generate_passwd(PASSWORD_SIZE);
                strncpy(chain.head, password.c_str(), sizeof(rtEntry::head));
                calculTail(password, chain.tail);
            }


            /**
             * @brief calculTail Calcul the tail of a password given.
             * @param password Password to calcul the tail
             * @return the tail of the passwords generated
             */
            constexpr void calculTail(string &password, char *tail)
            {

                for (unsigned step = 0; step < HASH_LEN; ++step)
                {
                    password = sha256(password);
                    REDUCE(password, tail, step);
                    password = string(tail, PASSWORD_SIZE);
                }


            }

            /**
             * @brief writePrecomputedValuesIntoTable Write all precomputed value of the rainbowTable
             * @param entries temporary set ordered of the rainbowTable
             */
            void writePrecomputedValuesIntoTable(const set<rtEntry> &entries);


            /**
             * @brief combineOrderedMiniTableIntoSet
             * Combine the different miniRainbowTable [Work of different thread] into a RainbowTable sorted.
             * @param filesName Array of different filesName of the miniRainbowTable.
             * @param set_table the set to put into all entry, to sort the table.
             * @param nb_table the number of miniRainbowTable.
             */
            void combineOrderedMiniTableIntoSet(const string *filesName, set<rtEntry> &set_table, unsigned nb_table);

        public :

            /**
             * @brief RainbowTableGen Constructor of a rainbowTable
             * @param size Size in MB of the table
             * @param pwdLength Length of passwords
             */
            RainbowTableGen(float size);

            /**
             * @brief RainbowTableGen destructor.
             */
            ~RainbowTableGen();

            /**
             * @brief generateTable Generate a rainbowTable
             */
            void generateTable();
    };
} // end namespace rainbow

#endif // RAINBOWTABLEGEN_HPP
