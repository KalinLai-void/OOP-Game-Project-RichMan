#ifndef CARDSTORE_HPP
#define CARDSTORE_HPP

#include "Bank.hpp"
#include "Card.hpp"
#include "Player.hpp"
#include <iostream>
#include <memory>
#include <vector>

class CardStore {
private:
    std::vector<Card> cards;

public:
    CardStore();
    void displayStore(std::shared_ptr<Player> player);
    void purchaseCard(std::shared_ptr<Player> player, const Card& card);
};

#endif // CARDSTORE_HPP
