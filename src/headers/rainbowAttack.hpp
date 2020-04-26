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



    std::optional<std::string> crackPassword(std::string & hash)
    const;
    std::optional<std::pair<std::string, std::string>> pwdBinarySearch(
                std::string & pwd) const;
    void getHashesToCrack(const std::string & filepath);


  public:


    RainbowAttack(std::string & hashToCrackFile);
    ~RainbowAttack();

    void attack();



};

} // end namespace rainbow

#endif // RAINBOWATTACK_HPP
