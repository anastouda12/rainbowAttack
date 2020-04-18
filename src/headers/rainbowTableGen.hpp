#ifndef RAINBOWTABLEGEN_HPP
#define RAINBOWTABLEGEN_HPP

#define HASH_LEN 50000
#define MINIMAL_PASSWORD_LENGTH 6
#define MAXIMAL_PASSWORD_LENGTH 8

#include <fstream>
#include <utility>
#include <vector>

namespace rainbow {

class RainbowTableGen
{

    /**
     * @brief Size on MegaByte of the rainbowTable.
     */
    float size_;

    /**
     * @brief tablePrecomputed Table precomputed of Head pwd and tail hash.
     */
    std::vector<std::pair<std::string,std::string>> tablePrecomputed_;

    /**
     * @brief File containing the rainbow table.
     */
    std::ofstream rainbowTableFile_;

public :

    RainbowTableGen(const float size);
    ~RainbowTableGen();
    void generateTable();
    void loadFromFile(const std::string& fileName);
    std::vector<std::pair<std::string,std::string>> getTablePrecomputed() const;

private:
    std::string calculTail(std::string password) const;
    std::pair<std::string,std::string> buildPrecomputedHashChain() const;
    void writePrecomputedValuesIntoTable();
    void insertToPrecomputedOrdered(const std::pair<std::string, std::string> & pair);
    int findIndexOrdered(const std::string & hash) const;


};

} // end namespace rainbow

#endif // RAINBOWTABLEGEN_HPP
