#include "RocketCard.hpp"
#include "InputManager.hpp"
#include <iostream>

void RocketCard::useEffect(std::vector<std::shared_ptr<Player>>& players, std::shared_ptr<Player> curPlayer) {
    if (players.size() < 2) {
        std::cout << "There are no other player to send to the hospital!" << std::endl;
        return;
    }
    std::cout << std::endl << "Choose a player to send to the hospital :" << std::endl;
    int idx = 1;
    std::vector<std::shared_ptr<Player>> availablePlayers;
    for (auto& player : players) {
        if (player != curPlayer) {
            std::cout << idx << ". " << player->getName() << std::endl;
            availablePlayers.push_back(player);
            idx++;
        }
    }
    std::cout << "Choose a player (1-" << static_cast<int>(availablePlayers.size()) << "): ";
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
            std::cout << "Invalid input. Please choose a player (1-" << static_cast<int>(availablePlayers.size()) << "): ";
        }
    }

    std::shared_ptr<Player> targetPlayer = availablePlayers[choice - 1];

    // Move to nerarest Hospital.
    TileAction action = TileAction::NONE;
    int step = 1;
    while (true) {
        int newPos = (targetPlayer->getPosition() + step) % Board::getInstance()->getSize();
        targetPlayer->setPosition(newPos);
        action = Board::getInstance()->getTile(targetPlayer->getPosition())->landOn(targetPlayer);
        if (action == TileAction::HOSPITAL) {
            break;
        }
    }
    targetPlayer->sendToHospital(2);

    Board::getInstance()->drawBoard(players);

    std::cout << targetPlayer->getName() << " was hit by Rocket and sent to the hospital for 2 turns!" << std::endl;
    std::cout << "It's " << curPlayer->getName() << "'s turn." << std::endl;
}