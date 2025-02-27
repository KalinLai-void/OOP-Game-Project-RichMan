#ifndef ROCKET_CARD_HPP
#define ROCKET_CARD_HPP

#include "Card.hpp"
#include "Player.hpp"
#include "Tile.hpp"
#include "Board.hpp"

class RocketCard : public Card {
public:
    RocketCard()
        : Card("Rocket Card", 3000, "Send a player to the hospital for 2 turns.") {}

    void useEffect(std::vector<std::shared_ptr<Player>>& players, std::shared_ptr<Player> curPlayer, Board& board) override;

};

#endif // !ROCKET_CARD_HPP
