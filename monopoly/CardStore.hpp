#ifndef CARDSTORE_HPP
#define CARDSTORE_HPP

#include "Bank.hpp"
#include "Card.hpp"
#include "Player.hpp"
#include "DiceControlCard.hpp"
#include "RocketCard.hpp"
#include "DestroyPropertyCard.hpp"
#include "DrawFateCard.hpp"
#include "BarrierCard.hpp"
#include <iostream>
#include <memory>
#include <vector>

class CardStore {
private:
    std::vector<std::shared_ptr<Card>> cards;

public:
    CardStore();
    void displayStore(std::shared_ptr<Player> player);
    void purchaseCard(std::shared_ptr<Player> player, std::shared_ptr<Card> card);
    std::vector<std::shared_ptr<Card>> getCards() const;
};

#endif // CARDSTORE_HPP
