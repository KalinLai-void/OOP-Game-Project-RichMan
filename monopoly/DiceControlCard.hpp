#ifndef DICECONTROL_CARD_HPP
#define DICECONTROL_CARD_HPP

#include "Card.hpp"
#include "Player.hpp"
#include "Board.hpp"

class DiceControlCard : public Card {
public:
    DiceControlCard()
        : Card("Dice Control Card", 2000, "Choose the number you roll on the dice.") {}

    void useEffect(std::vector<std::shared_ptr<Player>>& players, std::shared_ptr<Player> player, Board& board) override;
};

#endif // !DICECONTROL_CARD_HPP
