#ifndef RAINBOWATTACK_HPP
#define RAINBOWATTACK_HPP

#include <fstream>
#include <optional>
#include "rainbowTableGen.hpp"


namespace rainbow {

class RainbowAttack{

private:

    RainbowTableGen & rainbowTable_;
    std::vector<std::pair<std::string,std::string>> LoginAndHashToCrack_;



    std::optional<std::string> crackPassword(const std::string & hash) const;
    std::optional<std::pair<std::string,std::string>> pwdBinarySearch(const std::string & pwd) const;
    void initPasswordsToCrack(const std::string file);


public:


    RainbowAttack(RainbowTableGen & table, std::string pathPasswordsToCrack);
    ~RainbowAttack();

    void attack();



};

} // end namespace rainbow

#endif // RAINBOWATTACK_HPP
