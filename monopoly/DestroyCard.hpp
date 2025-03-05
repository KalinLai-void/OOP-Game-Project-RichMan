#ifndef DESTROY_CARD_HPP
#define DESTROY_CARD_HPP

#include "Card.hpp"
#include "Player.hpp"
#include "Tile.hpp"
#include "PropertyTile.hpp"
class  DestroyCard : public Card{
public:
    DestroyCard(const std::string& n, int p, const std::string& e)
        : Card(n, p, e) {}

    void useEffect(std::vector<std::shared_ptr<Player>>& players, std::shared_ptr<Player> curPlayer) override;
};


#endif // !DESTROY_CARD_HPP
