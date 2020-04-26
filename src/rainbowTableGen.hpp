#ifndef RAINBOWTABLEGEN_HPP
#define RAINBOWTABLEGEN_HPP

#define HASH_LEN 50000
#define MINIMAL_PASSWORD_LENGTH 6
#define MAXIMAL_PASSWORD_LENGTH 8
#define RTCHAIN_SIZE sizeof(rtChain)


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

    struct rtChain
    {
        char head[MAXIMAL_PASSWORD_LENGTH + 1];
        char tail[MAXIMAL_PASSWORD_LENGTH + 1];
    };

    RainbowTableGen(const float size, unsigned pwdLength);
    ~RainbowTableGen();
    void generateTable();

  private:
    std::string * calculTail(std::string & password) const;
    void buildPrecomputedHashChain(rtChain & rtChain) const;
    int findIndexOrdered(const std::string & tail,
                         std::vector<rtChain> & vec) const;

    inline void writePrecomputedValuesIntoTable(
        const std::vector<rtChain> & vec)
    {
        for (rtChain chain : vec)
        {
            this->rainbowTableFile_.write((char *) &chain, RTCHAIN_SIZE);
        }

    }

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
