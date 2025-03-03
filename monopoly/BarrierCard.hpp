#ifndef BARRIER_CARD
#define BARRIER_CARD

#include "Card.hpp"
#include "Player.hpp"

class BarrierCard : public Card {
public:
    BarrierCard()
        : Card("Barrier Card", 1500, "Place a barrier on a tile to blcok players.") {}

    void useEffect(std::vector<std::shared_ptr<Player>>& players, std::shared_ptr<Player> curPlayer) override;
};

#endif // !BARRIER_CARD
