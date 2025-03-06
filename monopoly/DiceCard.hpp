#ifndef DICE_CARD_HPP
#define DICE_CARD_HPP

#include "Card.hpp"
#include "Player.hpp"

class DiceCard : public Card {
public:
    DiceCard(const std::string& n, const std::string& i, int p, const std::string& e)
        : Card(n, i, p, e) {}

    void useEffect(std::vector<std::shared_ptr<Player>>& players, std::shared_ptr<Player> curPlayer) override;
};

#endif // !DICE_CARD_HPP
