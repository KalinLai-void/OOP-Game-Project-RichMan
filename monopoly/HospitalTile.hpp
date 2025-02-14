#ifndef HOSPITALTILE_HPP
#define HOSPITALTILE_HPP

#include "Tile.hpp"

class HospitalTile : public Tile {
public:
    HospitalTile(const std::string& n);
    void landOn(std::shared_ptr<Player> player) override;
};

#endif // HOSPITALTILE_HPP
