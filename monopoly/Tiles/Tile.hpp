#ifndef TILE_HPP
#define TILE_HPP

#include "Game/Player.hpp"
#include <memory>
#include <string>

/**
 * @enum TileAction
 * @brief Represents different actions triggered when a player lands on a tile.
 */
enum class TileAction {
    NONE,               ///< No special action
    PURCHASE_PROPERTY,  ///< Tile can be purchased
    OWN,                ///< Tile is already owned
    PAY_TOLL,           ///< Player must pay toll
    STORE,              ///< Tile represents a card store
    SPECIAL_EVENT,      ///< Triggers a random or special event
    TRADE,              ///< Trade opportunity
    HOSPITAL,           ///< Hospital tile (e.g., skip turns)
    START_POINT         ///< Starting tile
};


/**
 * Each tile has a unique ID, a name, and an optional block status.
 * Subclasses implement behavior for what happens when a player lands on the tile.
 */
class Tile {
protected:
    std::size_t id;   // Tile ID
    std::string name; // Tile name
    bool blocked;     // Whether the tile is currently blocked or inaccessible,
                      // e.g., used when a player uses a barrier card

public:
    Tile(const size_t id, const std::string& n)
        : id(id)
        , name(n)
        , blocked(false) {}
    virtual ~Tile() {}

    std::string getName() const {
        return name;
    }

    virtual std::string getNameWithId() const {
        return std::to_string(id) + " " + name;
    }

    size_t getId() const {
        return id;
    }

    void setBlock(bool b) {
        blocked = b;
    }

    bool isBlocked() const {
        return blocked;
    }
    
    /**
     * @brief Pure virtual function triggered when a player lands on the tile.
     * 
     * @param player The player who landed on the tile.
     * @return The action associated with landing on this tile.
     */
    virtual TileAction landOn(std::shared_ptr<Player> player) = 0;
};

#endif // TILE_HPP
