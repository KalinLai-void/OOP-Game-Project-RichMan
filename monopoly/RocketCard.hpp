#ifndef ROCKET_CARD_HPP
#define ROCKET_CARD_HPP

#include "Card.hpp"
#include "Player.hpp"

class RocketCard : public Card {
public:
    RocketCard()
        : Card("Rocket Card", 3000, "Move instantly to any tile.") {}

    void useEffect(std::shared_ptr<Player> player) override;

};

#endif // !ROCKET_CARD_HPP
