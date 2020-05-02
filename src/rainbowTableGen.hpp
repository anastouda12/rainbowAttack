#ifndef RAINBOWTABLEGEN_HPP
#define RAINBOWTABLEGEN_HPP

#include <fstream>
#include <utility>
#include <vector>
#include "src/reduction.hpp"
#include "src/utils.hpp"
#include "src/sha256.h"

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


    void generateMiniTable(std::string & fileName,
                           float TableSize);
    void combineOrderedMiniTableIntoVec(std::string
                                        filesName[],
                                        std::vector<RTChain> & vecToFill);


    /**
     * @brief calculTail Calcul the tail of a password given.
     * @param password Password to calcul the tail
     * @return the tail of the passwords
     */
    constexpr void calculTail(std::string & password, char * tail)
    const
    {
        for (unsigned i = 0; i < password.length(); i++)
        {
            tail[i] = password[i];
        }
        for (unsigned step = 0; step < HASH_LEN; ++step)
        {
            rainbow::reduce(sha256(tail), tail, step, 8);
        }
    }

    /**
     * @brief buildPrecomputedHashChain Construct a chain of the rainbowtable with reduce function and hash
     * @param rtChain Head and tails chains
     */
    inline void buildPrecomputedHashChain(RTChain & rtchain) const;

    /**
     * @brief findIndexOrdered Find index ordered with the tails to insert the tail given
     * @param tail tail to find index inside the table
     * @param vec temporary vector helping to sort the table
     * @return index inside the vector where the tails should be
     */
    constexpr int findIndexOrdered(const std::string & tail,
                                   std::vector<RTChain> & vec)const
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
        const std::vector<RTChain> & vec)
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
    constexpr void insertToPrecomputedOrdered(const RTChain & rtchain,
            std::vector <RTChain> & vec)
    {
        //Find the position where the pair should be inserted, and insert it. O(log n) time.
        int index = findIndexOrdered(rtchain.tail, vec);
        vec.insert(vec.begin() + index, rtchain);

    }
};
} // end namespace rainbow

#endif // RAINBOWTABLEGEN_HPP
