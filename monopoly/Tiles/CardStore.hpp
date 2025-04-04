#ifndef CARDSTORE_HPP
#define CARDSTORE_HPP

#include "Bank.hpp"
#include "Cards/Card.hpp"
#include "Game/GameConfig.hpp"
#include "Game/Player.hpp"
#include <iostream>
#include <memory>
#include <vector>

/**
 * Singleton class that stores available cards, displays them to players, and handles purchase logic.
 */
class CardStore {
private:
    static CardStore* instance;
    std::vector<std::shared_ptr<Card>> cards;

    /**
     * @brief Private constructor for singleton pattern.
     * 
     * @param config Game configuration used to initialize card store.
     */
    CardStore(const GameConfig& config);
    ~CardStore() = default;

public:
    static CardStore* getInstance(const GameConfig& config);
    static CardStore* getInstance();
    static void destroyInstance();

    CardStore(const CardStore&) = delete;
    CardStore& operator=(const CardStore&) = delete;

    /**
     * @brief Displays the available cards to the player.
     * 
     * @param player The player who is viewing the card store.
     */
    void displayStore(std::shared_ptr<Player> player);

    /**
     * @brief Handles the logic for purchasing a card.
     * 
     * Deducts funds, gives the card.
     * 
     * @param player The player attempting to purchase the card.
     * @param card The card the player wants to purchase.
     */
    void purchaseCard(std::shared_ptr<Player> player, std::shared_ptr<Card> card);

    /**
     * @brief Gets all available cards in the store.
     * 
     * @return A vector of shared pointers to cards.
     */
    std::vector<std::shared_ptr<Card>> getCards() const;

    /**
     * @brief Finds a card by its name.
     * 
     * @param cardName The name of the card to search for.
     * @return Shared pointer to the card if found, nullptr otherwise.
     */
    std::shared_ptr<Card> getCardByName(const std::string& cardName) const;
};

#endif // CARDSTORE_HPP


