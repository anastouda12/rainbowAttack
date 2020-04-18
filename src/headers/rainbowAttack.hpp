#ifndef RAINBOWATTACK_HPP
#define RAINBOWATTACK_HPP

#include <fstream>
#include <optional>
#include "rainbowTableGen.hpp"


namespace rainbow {

class RainbowAttack{

private:

    RainbowTableGen & rainbowTable_;



    std::optional<std::string> crackPassword(const std::string & hash) const;
    std::optional<std::pair<std::string,std::string>> pwdBinarySearch(const std::string & hash) const;


public:

     /**
     * @brief Constructs an instance of RainbowAttack with the specified rainbow
     * table.
     *
     * @param table rainbowtable.
     */
    RainbowAttack(RainbowTableGen & table);

       /**
       * @brief Attacks the given hash and stores the password at the given
       * address.
       *
       * @param hash is the given hash.
       * @param password is the given address.
       * @return true if the attack succeeded.
       */
      bool attack(const std::string &hash, std::string &password, unsigned length);

      //~RainbowAttack();


};

} // end namespace rainbow

#endif // RAINBOWATTACK_HPP
