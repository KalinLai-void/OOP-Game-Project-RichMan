#include "DrawFateCard.hpp"

void DrawFateCard::useEffect(std::vector<std::shared_ptr<Player>>& players, std::shared_ptr<Player> curPlayer) {
    std::cout << curPlayer->getName() << " used Draw Fate Card! A fate event will occur..." << std::endl;
    
    std::shared_ptr<EventTile> targetTile = nullptr;
    for (auto& tile : Board::getInstance()->getTileList()) {
        targetTile = std::dynamic_pointer_cast<EventTile>(tile);
        if (targetTile) {
            if (targetTile->getEventType() == EventType::FATE) {
                break;
            }
        }
    }

    if (targetTile) {
        std::cout << "You have drawn a Fate Event from the board" << std::endl;
        targetTile->triggerEvent(curPlayer);
    } else {
        std::cout << "No Fate event tile found on the board!" << std::endl;
    }
    return;
}