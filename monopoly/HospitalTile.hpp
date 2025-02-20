#ifndef HOSPITALTILE_HPP
#define HOSPITALTILE_HPP

#include "Tile.hpp"

class HospitalTile : public Tile {
private:
    int fee;

public:
    HospitalTile(const std::string& n);
    TileAction landOn(std::shared_ptr<Player> player) override;
    void handleHospitalChoice(std::shared_ptr<Player> player);
};

#endif // HOSPITALTILE_HPP
