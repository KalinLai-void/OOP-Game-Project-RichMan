#ifndef CARDSTORE_HPP
#define CARDSTORE_HPP

#include "Bank.hpp"
#include "BarrierCard.hpp"
#include "Card.hpp"
#include "DestroyCard.hpp"
#include "DiceCard.hpp"
#include "FateCard.hpp"
#include "GameConfig.hpp"
#include "Player.hpp"
#include "RocketCard.hpp"
#include <iostream>
#include <memory>
#include <vector>

class CardStore {
private:
    static CardStore* instance;
    std::vector<std::shared_ptr<Card>> cards;

    CardStore(const GameConfig& config);
    ~CardStore() = default;

public:
    static CardStore* getInstance(const GameConfig& config);
    static CardStore* getInstance();
    static void destroyInstance();

    CardStore(const CardStore&) = delete;
    CardStore& operator=(const CardStore&) = delete;

    void displayStore(std::shared_ptr<Player> player);
    void purchaseCard(std::shared_ptr<Player> player, std::shared_ptr<Card> card);
    std::vector<std::shared_ptr<Card>> getCards() const;
};

#endif // CARDSTORE_HPP
