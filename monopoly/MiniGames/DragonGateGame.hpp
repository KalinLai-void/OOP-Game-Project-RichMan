#ifndef DRAGON_GATE_HPP
#define DRAGON_GATE_HPP

#include "MiniGame.hpp"

class DragonGateGame : public MiniGame {
public:
    // Returns the amount of coins won (could be 0 or a positive value)
    long long playGame(long long betAmount) override;
};

#endif // !DRAGON_GATE_HPP