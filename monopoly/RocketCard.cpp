#include "RocketCard.hpp"
#include <iostream>

void RocketCard::useEffect(std::vector<std::shared_ptr<Player>>& players, std::shared_ptr<Player> curPlayer, Board& board) {
    if (players.size() < 2) {
        std::cout << "There are no other player to send to the hospital!" << std::endl;
        return;
    }
    std::cout << std::endl << "Choose a player to send to the hospital (Enter the number):";
    int idx = 1;
    std::vector<std::shared_ptr<Player>> availablePlayers;
    for (auto& player : players) {
        if (player != curPlayer) {
            std::cout << idx << ". " << player->getName() << std::endl;
            availablePlayers.push_back(player);
            idx++;
        }
    }
    int choice;

    while (true) {
        std::string input;
        input.clear();

        std::getline(std::cin, input);
        try {
            size_t pos;
            choice = std::stoi(input, &pos);
            if (choice >= 1 && choice <= static_cast<int>(availablePlayers.size())) {
                break;
            } else {
                std::cout << "Invalid input. Please enter a number between 1 and ." << static_cast<int>(availablePlayers.size()) << std::endl;
                
            }

        } catch (const std::exception&) {
            std::cout << "Invalid input. Please enter a number between 1 and ." << static_cast<int>(availablePlayers.size()) << std::endl;
            
        }
    }


    std::shared_ptr<Player> targetPlayer = availablePlayers[choice - 1];
    
    // Move to nerarest Hospital.
    TileAction action = TileAction::NONE;
    int step = 1;
    while (true) {
        int newPos = (targetPlayer->getPosition() + step) % board.getSize();
        targetPlayer->setPosition(newPos);
        action = board.getTile(targetPlayer->getPosition())->landOn(targetPlayer);
        if (action == TileAction::HOSPITAL) {
            break;
        }
    }
    targetPlayer->sendToHospital(2);

    board.drawBoard(players);
    
    std::cout << targetPlayer->getName() << " was hit by Rocket and sent to the hospital for 2 turns!" << std::endl;
    std::cout << "It's " << curPlayer->getName() << "'s turn." << std::endl;
}