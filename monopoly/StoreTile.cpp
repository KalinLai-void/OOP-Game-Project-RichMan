#include "StoreTile.hpp"
#include "Bank.hpp"
#include "CardStore.hpp"
#include <iostream>

using namespace std;

StoreTile::StoreTile(const size_t id, const std::string& n)
    : Tile(id, n) {}

CardStore* StoreTile::getStore() {
    return store;
}

void StoreTile::enterStore(std::shared_ptr<Player> player) {
    if (store) {
        store->displayStore(player);
    } else {
        std::cerr << "Error: store is null!" << std::endl;
    }
}

TileAction StoreTile::landOn(std::shared_ptr<Player> player) {
    return TileAction::STORE;
}
