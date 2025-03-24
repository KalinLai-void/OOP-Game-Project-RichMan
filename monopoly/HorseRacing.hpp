#ifndef HORSE_RACING_HPP
#define HORSE_RACING_HPP

#include "MiniGame.hpp"

class HorseRacing : public MiniGame {
public:
    // Takes the bet amount as input, returns positive value for winnings, negative for losses
    long long playGame(long long betAmount) override;
};

#endif // !HORSE_RACING_HPP