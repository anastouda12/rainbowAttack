#ifndef RAINBOWATTACK_HPP
#define RAINBOWATTACK_HPP

#include <fstream>
#include "rainbowTableGen.hpp"


namespace rainbow {

class RainbowAttack{

private:

    std::ifstream rainbowTable_;

    /**
     * @brief Tells if the given hash is in the specified hash chain.
     *
     * @param hash is the given hash;
     * @param size is the size of the specified hash chain.
     * @return true if the given hash is in the specified hash chain.
     */
    bool isInHashChain(std::string hash, std::string tail, size_t size) const;

    /**
     * @brief Builds up the password corresponding to the given hash in the
     * specified hash chain.
     *
     * This method should only be used when the hash is actually in the specified
     * hash chain.
     *
     * @param hash is the given hash.
     * @param head is the head of the specified hash chain.
     * @return the password corresponding to the given hash.
     */
    std::string buildPasswordUp(std::string hash, std::string head);

public:

     /**
      * @brief Constructs an instance of RainbowAttack with the specified rainbow
      * table path.
      *
      * @param rainbowTablePath is the specified path.
      * @throws exception if the path is not valid or the file cannot be open.
      */
     RainbowAttack(std::string rainbowTablePath)
     {
         rainbowTable_.open(rainbowTablePath, std::ios::binary);
      }


      /**
       * @brief Finds the table entry corresponding to the given tail. If no table
       * entry matches the given tail.
       *
       * In order to find the desired entry the method makes a dichotomic file
       * search.
       *
       * @param tail is the given tail.
       * @param entry is where the found entry will be stored.
       * @return the rainbow table entry corresponding to the given tail.
       * @throws invalid_argument if the given tail is not in the rainbow table.
       */
     // bool find(std::string tail, RainbowTableGen::TableEntry &entry);

        /**
         * @brief Attacks the given hash and stores the password at the given
         * address.
         *
         * @param hash is the given hash.
         * @param password is the given address.
         * @return true if the attack succeeded.
         */
        bool attack(const std::string &hash, std::string &password, unsigned length);

        ~RainbowAttack() { rainbowTable_.close(); }


};

} // end namespace rainbow

#endif // RAINBOWATTACK_HPP
