#ifndef DESTROY_CARD_HPP
#define DESTROY_CARD_HPP

#include "Card.hpp"
#include "Game/Player.hpp"
#include "Tiles/PropertyTile.hpp"
#include "Tiles/Tile.hpp"

class DestroyCard : public Card {
public:
    DestroyCard(const std::string& n, const std::string& i, int p, const std::string& e)
        : Card(n, i, p, e) {}

    void useEffect(std::vector<std::shared_ptr<Player>>& players, std::shared_ptr<Player> curPlayer) override;
};

#endif // !DESTROY_CARD_HPP
