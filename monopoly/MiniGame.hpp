#ifndef MINIGAME_HPP
#define MINIGAME_HPP

#include <iostream> 
#include <random>

class MiniGame {
public:
    virtual long long playGame(long long betAmount) = 0;
};

#endif // !MINIGAME_HPP