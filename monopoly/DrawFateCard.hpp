#ifndef DRAW_FATE_CARD_HPP
#define DRAW_FATE_CARD_HPP

#include "Card.hpp"
#include "Board.hpp"
#include "EventTile.hpp"
#include "Tile.hpp"
#include <memory>

class DrawFateCard : public Card {
public:
    DrawFateCard()
        : Card("Draw Fate Card", 1000, "Trigger a Fate event.") {}

    void useEffect(std::vector<std::shared_ptr<Player>>& players, std::shared_ptr<Player> curPlayer, Board& board) override;

};

#endif // !DRAW_FATE_CARD_HPP
