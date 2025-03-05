#ifndef BARRIER_CARD
#define BARRIER_CARD

#include "Card.hpp"
#include "Player.hpp"

class BarrierCard : public Card {
public:
    BarrierCard(const std::string& n, int p, const std::string& e)
        : Card(n, p, e) {}

    void useEffect(std::vector<std::shared_ptr<Player>>& players, std::shared_ptr<Player> curPlayer) override;
};

#endif // !BARRIER_CARD
