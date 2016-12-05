#ifndef __GAMECONFIGDATABASE_HPP__
#define __GAMECONFIGDATABASE_HPP__

#include "../model/GameConfig.hpp"

class GameConfigDatabase
{
public:
    bool Read(GameConfig& config);
    bool Write(GameConfig& config);
};

#endif  // __GAMECONFIGDATABASE_HPP__
