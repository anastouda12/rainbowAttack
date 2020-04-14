#ifndef RAINBOWTABLEGEN_HPP
#define RAINBOWTABLEGEN_HPP

#define HASH_LEN 50000
#define MINIMAL_PASSWORD_LENGTH 6
#define MAXIMAL_PASSWORD_LENGTH 8

#include <fstream>
#include <utility>

namespace rainbow {

class RainbowTableGen
{

    /**
     * @brief Size of the rainbowTable.
     */
    float size_;

    /**
     * @brief Outputstream of the file containing the rainbow table.
     */
    std::ofstream ofstreamRainbowTable_;

public :

    RainbowTableGen(const float size, std::string path);
    ~RainbowTableGen();
    void generate();
    void print();
    std::string getTail(std::string password) const;
    std::pair<std::string,std::string> buildPrecomputedHashChain() const;

};

} // end namespace rainbow

#endif // RAINBOWTABLEGEN_HPP
