#ifndef RAINBOWTABLEGEN_HPP
#define RAINBOWTABLEGEN_HPP

#include <fstream>
#include <utility>
#include <vector>
#include "src/reduction.hpp"
#include "src/utils.hpp"
#include "src/sha256.h"

#define printGenProgress(c_size, e_size)\
    std::cout << "[INFO] Generation rainbowTable : ";\
    std::cout << std::setfill('0')  << std::fixed << std::setprecision(4);\
    std::cout << (static_cast<double>(c_size) / static_cast<double>(e_size)) * 100 << " % ...\r" << std::flush;\

#define printEndGenerator(duration)                                        \
    std::cout << "[SUCCESS] : RainbowTable was generated on file 'Build/" << \
              FILE_NAME_RTABLE                                                 \
              << "'"                                                            \
              << std::endl;                                                      \
    std::cout << "[DURATION] : Generated on "; \
    std::cout << msToHours(duration.count()) << " hours : ";                        \
    std::cout << msToMinute(duration.count()) << " minutes : ";                     \
    std::cout << msToSecond(duration.count()) << " seconds" << std::endl;           \


#define buildPrecomputedHashChain(rtchain)   \
    std::string && password = generate_passwd(MAXIMAL_PASSWORD_LENGTH); \
    char tail[MAXIMAL_PASSWORD_LENGTH+1] ="";                           \
    copyArrays(password.c_str(),rtchain.head);\
    calculTail(std::move(password), tail); \
    copyArrays(tail,rtchain.tail);\


#define combineOrderedMiniTableIntoVec(filesName, vecToFill)    \
    for (unsigned i = 0; i < num_cpus; ++i){\
        std::ifstream table(*(filesName + i),std::ios_base::in | std::ios_base::binary);\
        if (table.is_open()){ \
            RTChain chain;  \
            unsigned end = table.seekg(0, std::ios::end).tellg() / RTCHAIN_SIZE;\
            table.seekg(0, std::ios::beg);  \
            for (unsigned i = 0; i <= end; i++){  \
                table.seekg(i * RTCHAIN_SIZE);  \
                table.read((char *) &chain, RTCHAIN_SIZE);  \
                insertToPrecomputedOrdered(chain, vecToFill);   \
            }\
            table.close();\
        }\
    }\

namespace rainbow
{

    class RainbowTableGen
    {

            /**
             * @brief Size on MegaByte of the rainbowTable.
             */
            float size_;


            float currentSizeGeneration_;

            /**
             * @brief File containing the rainbow table.
             */
            std::ofstream rainbowTableFile_;

        public :

            /**
             * @brief RainbowTableGen Constructor of a rainbowTable
             * @param size Size in MB of the table
             * @param pwdLength Length of passwords
             */
            RainbowTableGen(const float size);

            ~RainbowTableGen();

            /**
             * @brief generateTable Generate a rainbowTable
             */
            void generateTable();

        private:


            void generateMiniTable(const std::string &fileName,
                                   float TableSize);

            /**
             * @brief calculTail Calcul the tail of a password given.
             * @param password Password to calcul the tail
             * @return the tail of the passwords
             */
            constexpr void calculTail(const std::string &&password, char *tail)
            const
            {
                copyArrays(password.c_str(), tail);

                for (unsigned step = 0; step < HASH_LEN; ++step)
                {
                    rainbow::reduce(sha256(tail), tail, step, 8);
                }
            }


            /**
             * @brief findIndexOrdered Find index ordered with the tails to insert the tail given
             * @param tail tail to find index inside the table
             * @param vec temporary vector helping to sort the table
             * @return index inside the vector where the tails should be
             */
            constexpr int findIndexOrdered(const std::string &tail,
                                           std::vector<RTChain> &vec)const
            {
                // Searches for tail using the binary search algorithm.
                int min = 0, max = vec.size() - 1;
                int mid = (min + max) >> 1, comp = 0;

                while (min <= max)
                {
                    comp = tail.compare(vec[mid].tail);

                    if (comp == 0)
                    {
                        return mid;
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

                return min;
            }

            /**
             * @brief writePrecomputedValuesIntoTable Write all precomputed value of the rainbowTable
             * @param vec temporary vector of the rainbowTable
             */
            constexpr void writePrecomputedValuesIntoTable(
                    const std::vector<RTChain> &&vec)
            {
                rainbowTableFile_.open(FILE_NAME_RTABLE,
                                       std::ios::out | std::ios::binary | std::ios::trunc);

                if (rainbowTableFile_.is_open())
                {
                    for (RTChain chain : vec)
                    {
                        this->rainbowTableFile_.write((char *) &chain, RTCHAIN_SIZE);
                    }

                    rainbowTableFile_.close();
                }

            }

            /**
             * @brief insertToPrecomputedOrdered Insert value ordered inside a temporary vector to sort the table with the Tails of chains
             * @param rtchain Chains to insert into the vector
             * @param vec temporary vector to sort the table
             */
            constexpr void insertToPrecomputedOrdered(const RTChain &rtchain,
                            std::vector <RTChain> &vec)
            {
                //Find the position where the pair should be inserted, and insert it. O(log n) time.
                int index = findIndexOrdered(rtchain.tail, vec);
                vec.insert(vec.begin() + index, rtchain);

            }
    };
} // end namespace rainbow

#endif // RAINBOWTABLEGEN_HPP
