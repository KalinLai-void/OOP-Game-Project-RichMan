#ifndef CARDSTORE_HPP
#define CARDSTORE_HPP

#include "Bank.hpp"
#include "BarrierCard.hpp"
#include "Card.hpp"
#include "DestroyPropertyCard.hpp"
#include "DiceControlCard.hpp"
#include "DrawFateCard.hpp"
#include "Player.hpp"
#include "RocketCard.hpp"
#include <iostream>
#include <memory>
#include <vector>

class CardStore {
private:
    static CardStore* instance;
    std::vector<std::shared_ptr<Card>> cards;

    CardStore();
    ~CardStore() = default;

public:
    static CardStore* getInstance();
    static void destroyInstance();

    CardStore(const CardStore&) = delete;
    CardStore& operator=(const CardStore&) = delete;

    void displayStore(std::shared_ptr<Player> player);
    void purchaseCard(std::shared_ptr<Player> player, std::shared_ptr<Card> card);
    std::vector<std::shared_ptr<Card>> getCards() const;
};

#endif // CARDSTORE_HPP
