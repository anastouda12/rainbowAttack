#ifndef RAINBOWTABLEGEN_HPP
#define RAINBOWTABLEGEN_HPP

#include <fstream>
#include <set>
#include "src/reduction.hpp"
#include "src/utils.hpp"
#include "src/sha256.h"

#define printGenProgress(c_size, e_size)\
    std::cout << "[INFO] Generation rainbowTable : ";\
    std::cout << std::setfill('0')  << std::fixed << std::setprecision(4);\
    std::cout << (static_cast<double>(c_size) / static_cast<double>(e_size)) * 100 << " % ...\r" << std::flush;\

#define printEndGenerator(ms)                                        \
    std::cout << "[SUCCESS] : RainbowTable was generated on file 'Build/" \
              << FILE_NAME_RTABLE \
              << "'"         \
              << std::endl;     \
    std::cout << "[DURATION] : Generated on ";   \
    std::cout << msToHours(ms) << " hours : ";   \
    std::cout << msToMinute(ms) << " minutes : ";   \
    std::cout << msToSecond(ms) << " seconds" << std::endl;  \


#define buildPrecomputedHashChain(rtchain)   \
    std::string && password = generate_passwd(PASSWORD_SIZE); \
    copyArrays(password.c_str(),rtchain.head,PASSWORD_SIZE);\
    calculTail(password, rtchain.tail); \

#define combineOrderedMiniTableIntoSet(filesName, set_table)    \
    for (unsigned i = 0; i < num_cpus; ++i){\
        std::ifstream table(*(filesName + i),std::ios_base::in | std::ios_base::binary);\
        if (table.is_open()){ \
            RTChain chain;  \
            unsigned end = table.seekg(0, std::ios::end).tellg() / RTCHAIN_SIZE;\
            table.seekg(0, std::ios::beg);  \
            for (unsigned i = 0; i < end; ++i){  \
                table.seekg(i * RTCHAIN_SIZE);  \
                table.read((char *) &chain, RTCHAIN_SIZE);  \
                set_table.insert(std::move(chain));\
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
            RainbowTableGen(float &&size);

            ~RainbowTableGen();

            /**
             * @brief generateTable Generate a rainbowTable
             */
            void generateTable();

        private:


            void generateMiniTable(const std::string &fileName,
                                   const int nbchain_by_cpu);


            /**
             * @brief calculTail Calcul the tail of a password given.
             * @param password Password to calcul the tail
             * @return the tail of the passwords
             */
            constexpr void calculTail(const std::string &password, char *tail)
            const
            {
                copyArrays(password.c_str(), tail, PASSWORD_SIZE);

                for (unsigned step = 0; step < HASH_LEN; ++step)
                {
                    reduce(sha256(tail), tail, step, PASSWORD_SIZE);
                }
            }

            /**
             * @brief writePrecomputedValuesIntoTable Write all precomputed value of the rainbowTable
             * @param entries temporary set ordered of the rainbowTable
             */
            inline void writePrecomputedValuesIntoTable(
                    const std::set<RTChain> &entries)
            {
                rainbowTableFile_.open(FILE_NAME_RTABLE,
                                       std::ios::out | std::ios::binary | std::ios::trunc);

                if (rainbowTableFile_.is_open())
                {

                    for (auto chain : entries)
                    {
                        this->rainbowTableFile_.write((char *) &chain, RTCHAIN_SIZE);
                    }

                    rainbowTableFile_.close();
                }

            }
    };
} // end namespace rainbow

#endif // RAINBOWTABLEGEN_HPP
