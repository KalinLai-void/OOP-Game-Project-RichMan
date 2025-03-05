#include "CardStore.hpp"
#include "SingletonManager.hpp"
#include <limits>

CardStore* CardStore::instance = nullptr;

CardStore::CardStore(const GameConfig& config) {

    for (const auto& card : config.getCards()) {
        std::cout << card.name << std::endl;
        if (card.name == "Barrier Card") {
            cards.push_back(std::make_shared<BarrierCard>(card.name, card.price, card.effect));
        } else if (card.name == "Destory Card") {
            cards.push_back(std::make_shared<DestroyCard>(card.name, card.price, card.effect));
        } else if (card.name == "Dice Card") {
            cards.push_back(std::make_shared<DiceCard>(card.name, card.price, card.effect));
        } else if (card.name == "Fate Card") {
            cards.push_back(std::make_shared<FateCard>(card.name, card.price, card.effect));
        } else if (card.name == "Rocket Card") {
            cards.push_back(std::make_shared<RocketCard>(card.name, card.price, card.effect));
        }
    }

    SingletonManager::registerDestructor(CardStore::destroyInstance);
}

CardStore* CardStore::getInstance(const GameConfig& config) {
    if (instance == nullptr) {
        instance = new CardStore(config);
    }
    return instance;
}

void CardStore::destroyInstance() {
    if (instance) {
        delete instance;
        instance = nullptr;
    }
}

void CardStore::displayStore(std::shared_ptr<Player> player) {
    std::cout << "=== Welcome to the Card Store ===" << std::endl;
    for (size_t i = 0; i < cards.size(); ++i) {
        std::cout << "[" << i + 1 << "] " << cards[i]->getName() << " - Price: $" << cards[i]->getPrice() << " - Effect: " << cards[i]->getEffect()
                  << std::endl;
    }
    std::cout << "[0] Exit store" << std::endl;
    std::cout << "Enter the number of the card you want to buy: ";

    int choice;
    while (true) {
        std::string input;
        input.clear();

        std::getline(std::cin, input);
        try {
            size_t pos;
            choice = std::stoi(input, &pos);
            if (choice == 0) {
                std::cout << "You left the store." << std::endl;
                return;
            }

            if (choice > 0 && choice <= static_cast<int>(cards.size())) {
                purchaseCard(player, cards[choice - 1]);
                return;
            } else {
                std::cout << "Invalid choice. Try again." << std::endl;
                std::cout << "Enter the number of the card you want to buy: ";
            }

        } catch (const std::exception&) {
            std::cout << "Invalid choice. Try again." << std::endl;
            std::cout << "Enter the number of the card you want to buy: ";
        }
    }
}

void CardStore::purchaseCard(std::shared_ptr<Player> player, std::shared_ptr<Card> card) {
    if (player->getMoney() >= card->getPrice()) {
        Bank::getInstance()->receiveFromPlayer(player, card->getPrice());
        player->addCard(card);
        std::cout << "You bought [" << card->getName() << "] for $" << card->getPrice() << "!" << std::endl;
    } else {
        std::cout << "You don't have enough money to buy [" << card->getName() << "]. Your balance: $" << player->getMoney() << ", but the card costs $"
                  << card->getPrice() << "." << std::endl;
    }
}

std::vector<std::shared_ptr<Card>> CardStore::getCards() const {
    return cards;
}
