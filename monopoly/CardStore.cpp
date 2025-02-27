﻿#include "CardStore.hpp"

CardStore::CardStore() {
    cards.emplace_back("Dice Control Card", 2000, "Choose the number you roll on the dice.");
    cards.emplace_back("Barrier Card", 1500, "Place a barrier on the board to block players.");
    cards.emplace_back("Rocket Card", 3000, "Move instantly to any tile.");
}

void CardStore::displayStore(std::shared_ptr<Player> player) {
    std::cout << "=== Welcome to the Card Store ===" << std::endl;
    for (size_t i = 0; i < cards.size(); ++i) {
        std::cout << "[" << i + 1 << "] " << cards[i].getName() << " - Price: $" << cards[i].getPrice() << " - Effect: " << cards[i].getEffect() << std::endl;
    }
    std::cout << "[0] Exit store" << std::endl;
    std::cout << "Enter the number of the card you want to buy: ";

    int choice;
    std::cin >> choice;

    if (choice == 0) {
        std::cout << "You left the store." << std::endl;
        return;
    }

    if (choice > 0 && choice <= static_cast<int>(cards.size())) {
        purchaseCard(player, cards[choice - 1]);
    } else {
        std::cout << "Invalid choice. Try again." << std::endl;
    }
}

void CardStore::purchaseCard(std::shared_ptr<Player> player, const Card& card) {
    if (player->getMoney() >= card.getPrice()) {
        player->deductMoney(card.getPrice());
        Bank::getInstance()->receiveFromPlayer(card.getPrice());
        player->addCard(card);
        std::cout << "You bought [" << card.getName() << "] for $" << card.getPrice() << "!" << std::endl;
    } else {
        std::cout << "You don't have enough money to buy [" << card.getName() << "]." << std::endl;
    }
}
