#ifndef DESTROY_PROPERTY_CARD_HPP
#define DESTROY_PROPERTY_CARD_HPP

#include "Card.hpp"
#include "Player.hpp"
#include "Tile.hpp"
#include "PropertyTile.hpp"
class  DestroyPropertyCard : public Card{
public:
    DestroyPropertyCard()
        : Card("Destory Property Card", 2500, "Destory another player's property.") {}

    void useEffect(std::vector<std::shared_ptr<Player>>& players, std::shared_ptr<Player> curPlayer) override;
};


#endif // !DESTROY_PROPERTY_CARD_HPP
