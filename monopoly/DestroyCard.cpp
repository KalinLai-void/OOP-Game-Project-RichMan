#include "DestroyCard.hpp"
#include "CardStore.hpp"
#include "InputManager.hpp"

void DestroyCard::useEffect(std::vector<std::shared_ptr<Player>>& players, std::shared_ptr<Player> curPlayer) {
    if (players.size() < 2) {
        std::cout << "There are no other players to destroy properties from!" << std::endl;
        return;
    }

    // Select a player
    std::cout << std::endl << "Choose a player whose property you want to destroy: " << std::endl;
    int count = 1;
    std::vector<std::shared_ptr<Player>> availablePlayers;

    for (auto& player : players) {
        if (player != curPlayer) {
            std::cout << count << ". " << player->getName() << std::endl;
            availablePlayers.push_back(player);
            count++;
        }
    }
    std::cout << "Enter the number of the player whose property you want to destroy: ";
    int choice;
    while (true) {
        try {
            choice = InputManager::getKeyInt();
            std::cout << choice << std::endl;
            if (choice < 1 || choice > static_cast<int>(availablePlayers.size())) {
                throw std::invalid_argument("Invalid input");
            }
            break;

        } catch (const std::exception&) {
            std::cout << "Invalid input. Please enter a number between 1 and " << availablePlayers.size() << ":";
        }
    }

    std::shared_ptr<Player> targetPlayer = availablePlayers[choice - 1];

    // Get all property player owned
    std::vector<std::shared_ptr<PropertyTile>> playerProperties;
    for (auto& tile : Board::getInstance()->getTileList()) {
        std::shared_ptr<PropertyTile> propertyTile = std::dynamic_pointer_cast<PropertyTile>(tile);
        if (propertyTile && propertyTile->getPropertyOwner() == targetPlayer) {
            playerProperties.push_back(propertyTile);
        }
    }

    if (playerProperties.size() < 1) {
        std::cout << std::endl << targetPlayer->getName() << " doesn't have any property!." << std::endl;
        curPlayer->addCard(CardStore::getInstance()->getCardByName("Destroy Card"));
        return;
    }

    // Select a property to destroy a level
    std::cout << std::endl << targetPlayer->getName() << " has: " << std::endl;
    for (size_t i = 0; i < playerProperties.size(); i++) {
        std::cout << i + 1 << ". " << playerProperties[i]->getName() << " (Level: " << static_cast<int>(playerProperties[i]->getPropertyLevel()) << ")"
                  << std::endl;
    }

    std::cout << "Choose a property you want to destroy a level:";
    int propertyChoice;
    while (true) {

        std::cin >> propertyChoice;
        try {
            if (propertyChoice < 1 || propertyChoice > static_cast<int>(playerProperties.size())) {
                throw std::invalid_argument("Invalid input");
            }
            break;

        } catch (const std::exception&) {
            std::cout << "Invalid input. Please enter a number between 1 and " << playerProperties.size() << ": ";
        }
    }

    std::shared_ptr<PropertyTile> selectedProperty = playerProperties[propertyChoice - 1];

    // Destroy a level of selected property
    selectedProperty->downgrade();

    if (selectedProperty->getPropertyLevel() == PropertyLevel::EMPTY) {
        std::cout << selectedProperty->getName() << " has been completely destroyed and is now an empty space!" << std::endl;
    } else {
        std::cout << selectedProperty->getName() << " was downgraded (Current Level: " << static_cast<int>(selectedProperty->getPropertyLevel()) << ")"
                  << std::endl;
    }

    return;
}
