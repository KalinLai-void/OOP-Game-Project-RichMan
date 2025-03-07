#include "CardStore.hpp"
#include "InputManager.hpp"
#include "SingletonManager.hpp"
#include <limits>

CardStore* CardStore::instance = nullptr;

CardStore::CardStore(const GameConfig& config) {

    for (const auto& card : config.getCards()) {
        if (card.name == "Barrier Card") {
            cards.push_back(std::make_shared<BarrierCard>(card.name, card.icon, card.price, card.effect));
        } else if (card.name == "Destroy Card") {
            cards.push_back(std::make_shared<DestroyCard>(card.name, card.icon, card.price, card.effect));
        } else if (card.name == "Dice Card") {
            cards.push_back(std::make_shared<DiceCard>(card.name, card.icon, card.price, card.effect));
        } else if (card.name == "Fate Card") {
            cards.push_back(std::make_shared<FateCard>(card.name, card.icon, card.price, card.effect));
        } else if (card.name == "Rocket Card") {
            cards.push_back(std::make_shared<RocketCard>(card.name, card.icon, card.price, card.effect));
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

CardStore* CardStore::getInstance() {
    if (instance == nullptr) {
        throw std::runtime_error("CardStore instance is not initialized!");
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
        std::cout << "[" << i + 1 << "] " << std::setw(15) << cards[i]->getName() << " - Price: $" << std::setw(10) << cards[i]->getPrice()
                  << " - Effect: " << cards[i]->getEffect() << std::endl;
    }
    std::cout << "[0] Exit store" << std::endl;
    std::cout << "Enter the number of the card you want to buy: ";

    int choice;
    while (true) {
        try {
            choice = InputManager::getKeyInt();
            std::cout << choice << std::endl;
            if (choice == 0) {
                std::cout << "You left the store." << std::endl;
                return;
            }
            if (choice >= 0 && choice <= static_cast<int>(cards.size())) {

                purchaseCard(player, cards[choice - 1]); // convert char to int
                break;
            } else {
                std::cout << "Invalid choice. Try again." << std::endl;
                std::cout << "Enter the number of the card you want to buy: ";
            }

        } catch (const std::exception&) {
            std::cout << "Invalid choice. Try again." << std::endl;
            std::cout << "Enter the number of the card you want to buy: ";
        }
    }

    std::cout << "\nPress any key to continue...";
    InputManager::getKey();
    return;
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

std::shared_ptr<Card> CardStore::getCardByName(const std::string& cardName) const {
    for (auto& card : CardStore::getInstance()->getCards()) {
        if (card->getName() == cardName) {
            return card;
        }
    }
    return nullptr;
}