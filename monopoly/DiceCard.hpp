#ifndef DICE_CARD_HPP
#define DICE_CARD_HPP

#include "Card.hpp"
#include "Player.hpp"

class DiceCard : public Card {
public:
    DiceCard()
        : Card("Dice Card", 2000, "Choose the number you roll on the dice.") {}

    void useEffect(std::vector<std::shared_ptr<Player>>& players, std::shared_ptr<Player> curPlayer) override;
};

#endif // !DICE_CARD_HPP
