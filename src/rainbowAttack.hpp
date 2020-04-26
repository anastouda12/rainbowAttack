#ifndef RAINBOWATTACK_HPP
#define RAINBOWATTACK_HPP

#include <fstream>
#include <optional>
#include "rainbowTableGen.hpp"


namespace rainbow
{

class RainbowAttack
{

  private:

    /**
     * @brief File containing the rainbow table.
     */
    std::ifstream rainbowTableFile_;

    /**
     * @brief LoginAndHashToCrack_ All pair login-hash to crack inside 'HashToCrackFile' given.
     */
    std::vector<std::pair<std::string, std::string>> LoginAndHashToCrack_;

    /**
     * @brief passwordCracked_ File containing the passwords cracked
     */
    std::ofstream passwordCracked_;



    /**
     * @brief crackPassword Crack hash of a password
     * @param hash hash to retrieve password
     * @return password cracked if not empty string
     */
    std::string crackPassword(std::string & hash);

    /**
     * @brief pwdBinarySearch Binary search inside the rainbowtable to retrieves the chain who has the tail reduce given in param
     * @param tail the reduction given to checks in the rainbowtable
     * @return the chain who has the reduction in the assembly of the chain, if not empty string
     */
    RainbowTableGen::rtChain pwdBinarySearch(
        std::string & reduction);

    /**
     * @brief getHashesToCrack Retrieves the login hash pair to crack
     * @param filepath filepath to the file full of hashes
     */
    void getHashesToCrack(const std::string & filepath);


  public:


    /**
     * @brief RainbowAttack Constructor of the rainbowAttack
     * @param hashToCrackFile file containing the hashes to crack
     */
    RainbowAttack(std::string & hashToCrackFile);
    ~RainbowAttack();

    /**
     * @brief attack Attempt a rainbowAttack
     */
    void attack();



};

} // end namespace rainbow

#endif // RAINBOWATTACK_HPP
