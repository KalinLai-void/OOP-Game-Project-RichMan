#ifndef FATE_CARD_HPP
#define FATE_CARD_HPP

#include "Card.hpp"
#include "EventTile.hpp"
#include "Tile.hpp"
#include <memory>

class FateCard : public Card {
public:
    FateCard()
        : Card("Fate Card", 1000, "Trigger a Fate event.") {}

    void useEffect(std::vector<std::shared_ptr<Player>>& players, std::shared_ptr<Player> curPlayer) override;

};

#endif // !FATE_CARD_HPP
