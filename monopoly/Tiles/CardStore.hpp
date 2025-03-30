#ifndef CARDSTORE_HPP
#define CARDSTORE_HPP

#include "Bank.hpp"
#include "Cards/Card.hpp"
#include "Game/GameConfig.hpp"
#include "Game/Player.hpp"
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
    std::shared_ptr<Card> getCardByName(const std::string& cardName) const;
};

#endif // CARDSTORE_HPP
