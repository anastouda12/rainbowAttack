#ifndef RAINBOWTABLEGEN_HPP
#define RAINBOWTABLEGEN_HPP

#define HASH_LEN 50000
#define MINIMAL_PASSWORD_LENGTH 6
#define MAXIMAL_PASSWORD_LENGTH 8

#include <fstream>
#include <utility>
#include <set>

namespace rainbow {

class RainbowTableGen
{

    /**
     * @brief Size on MegaByte of the rainbowTable.
     */
    float size_;

    /**
     * @brief File containing the rainbow table.
     */
    std::ofstream rainbowTableFile_;

public :

    RainbowTableGen(const float size);
    ~RainbowTableGen();
    void generateTable();

private:
    std::string calculTail(std::string password) const;
    std::pair<std::string,std::string> buildPrecomputedHashChain() const;
    void writePrecomputedValuesIntoTable(std::ofstream & table, std::set<std::pair<std::string,std::string>> precomputedValues);

};

} // end namespace rainbow

#endif // RAINBOWTABLEGEN_HPP
