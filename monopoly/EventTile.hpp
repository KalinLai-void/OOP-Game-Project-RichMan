#ifndef EVENTTILE_HPP
#define EVENTTILE_HPP

#include "Tile.hpp"
#include <map>

enum class EventType { FATE, CHANCE };
class EventTile : public Tile {
private:
    EventType type;
    std::map<std::string, std::pair<int, int>> eventValueRange;

public:
    EventTile(const size_t id, const std::string& n, EventType t, const std::map<std::string, std::pair<int, int>>& valueRange);
    TileAction landOn(std::shared_ptr<Player> player) override;
    void triggerEvent(std::shared_ptr<Player> player);
    int getRandomValue100Multiple(int min, int amx);
    int getRandomValue100Multiple(EventType type);
};
#endif // EVENTTILE_HPP
