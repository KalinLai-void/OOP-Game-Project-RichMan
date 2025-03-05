#ifndef ROCKET_CARD_HPP
#define ROCKET_CARD_HPP

#include "Card.hpp"
#include "Player.hpp"
#include "Tile.hpp"

class RocketCard : public Card {
public:
    RocketCard(const std::string& n, int p, const std::string& e)
        : Card(n, p, e) {}

    void useEffect(std::vector<std::shared_ptr<Player>>& players, std::shared_ptr<Player> curPlayer) override;

};

#endif // !ROCKET_CARD_HPP
