#ifndef ROCKET_CARD_HPP
#define ROCKET_CARD_HPP

#include "Card.hpp"
#include "Game/Player.hpp"
#include "Tiles/Tile.hpp"

class RocketCard : public Card {
public:
    RocketCard(const std::string& n, const std::string& i, int p, const std::string& e)
        : Card(n, i, p, e) {}

    void useEffect(std::vector<std::shared_ptr<Player>>& players, std::shared_ptr<Player> curPlayer) override;

};

#endif // !ROCKET_CARD_HPP
