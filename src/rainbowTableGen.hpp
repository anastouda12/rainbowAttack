#ifndef RAINBOWTABLEGEN_HPP
#define RAINBOWTABLEGEN_HPP

#define HASH_LEN 50000
#define MINIMAL_PASSWORD_LENGTH 6
#define MAXIMAL_PASSWORD_LENGTH 8
#define RTCHAIN_SIZE sizeof(RainbowTableGen::rtChain)


#include <fstream>
#include <utility>
#include <vector>
#include "src/reduction.hpp"


namespace rainbow
{

class RainbowTableGen
{

    /**
     * @brief Size on MegaByte of the rainbowTable.
     */
    float size_;


    /**
     * @brief pwdLength_ Length of the password
     */
    unsigned pwdLength_;


    /**
     * @brief reduction_ reduction function
     */
    Reduction reduction_;


    /**
     * @brief File containing the rainbow table.
     */
    std::ofstream rainbowTableFile_;

  public :

    /**
     * @brief The rtChain struct Chain of a rainbowTable with the head and the tail
     */
    struct rtChain
    {
        char head[MAXIMAL_PASSWORD_LENGTH + 1];
        char tail[MAXIMAL_PASSWORD_LENGTH + 1];
    };

    /**
     * @brief RainbowTableGen Constructor of a rainbowTable
     * @param size Size in MB of the table
     * @param pwdLength Length of passwords
     */
    RainbowTableGen(const float size, unsigned pwdLength);

    ~RainbowTableGen();

    /**
     * @brief generateTable Generate a rainbowTable
     */
    void generateTable();

  private:
    /**
     * @brief calculTail Calcul the tail of a password given.
     * @param password Password to calcul the tail
     * @return the tail of the passwords
     */
    std::string * calculTail(std::string & password) const;

    /**
     * @brief buildPrecomputedHashChain Construct a chain of the rainbowtable with reduce function and hash
     * @param rtChain Head and tails chains
     */
    void buildPrecomputedHashChain(rtChain & rtChain) const;

    /**
     * @brief findIndexOrdered Find index ordered with the tails to insert the tail given
     * @param tail tail to find index inside the table
     * @param vec temporary vector helping to sort the table
     * @return index inside the vector where the tails should be
     */
    int findIndexOrdered(const std::string & tail,
                         std::vector<rtChain> & vec) const;

    /**
     * @brief writePrecomputedValuesIntoTable Write all precomputed value of the rainbowTable
     * @param vec temporary vector of the rainbowTable
     */
    inline void writePrecomputedValuesIntoTable(
        const std::vector<rtChain> & vec)
    {
        for (rtChain chain : vec)
        {
            this->rainbowTableFile_.write((char *) &chain, RTCHAIN_SIZE);
        }

    }

    /**
     * @brief insertToPrecomputedOrdered Insert value ordered inside a temporary vector to sort the table with the Tails of chains
     * @param rtchain Chains to insert into the vector
     * @param vec temporary vector to sort the table
     */
    inline void insertToPrecomputedOrdered(const rtChain & rtchain,
                                           std::vector <rtChain> & vec)
    {
        //Find the position where the pair should be inserted, and insert it. O(log n) time.
        int index = findIndexOrdered(rtchain.tail, vec);
        vec.insert(vec.begin() + index, rtchain);

    }
};
} // end namespace rainbow

#endif // RAINBOWTABLEGEN_HPP
