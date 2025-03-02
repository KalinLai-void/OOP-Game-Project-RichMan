#include "CardStore.hpp"

CardStore::CardStore() {
    cards.push_back(std::make_shared<DiceControlCard>());
    cards.push_back(std::make_shared<RocketCard>());
    cards.push_back(std::make_shared<DestroyPropertyCard>());
    cards.push_back(std::make_shared<DrawFateCard>());
    cards.push_back(std::make_shared<BarrierCard>());
    /*
    cards.emplace_back("Barrier Card", 1500, "Place a barrier on the board to block players.");
    */
}

void CardStore::displayStore(std::shared_ptr<Player> player) {
    std::cout << "=== Welcome to the Card Store ===" << std::endl;
    for (size_t i = 0; i < cards.size(); ++i) {
        std::cout << "[" << i + 1 << "] " << cards[i]->getName() << " - Price: $" << cards[i]->getPrice() << " - Effect: " << cards[i]->getEffect() << std::endl;
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
        std::cout << "You don't have enough money to buy [" << card->getName() << "]." << std::endl;
    }
}

std::vector<std::shared_ptr<Card>> CardStore::getCards() const {
    return cards;
}