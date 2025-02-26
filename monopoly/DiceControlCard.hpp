#ifndef DICECONTROL_CARD_HPP
#define DICECONTROL_CARD_HPP

#include "Card.hpp"
#include "Player.hpp"

class DiceControlCard : public Card {
public:
    DiceControlCard()
        : Card("Dice Control Card", 2000, "Choose the number you roll on the dice.") {}

    void useEffect(std::shared_ptr<Player> player) override;
};

#endif // !DICECONTROL_CARD_HPP
