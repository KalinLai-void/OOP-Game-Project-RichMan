#ifndef CARD_HPP
#define CARD_HPP

#include <vector>
#include <string>
#include <memory>
//#include "Board.hpp"

class Player; // Forward declaration to prevent circular dependency
class Board;

class Card {
private:
    std::string name;
    int price;
    std::string effect; // Card effect

public:
    Card(const std::string& n, int p, const std::string& e)
        : name(n), price(p), effect(e) {}

    virtual ~Card() = default;

    std::string getName() const {
        return name;
    }
    int getPrice() const {
        return price;
    }
    std::string getEffect() const {
        return effect;
    }

    virtual void useEffect(std::vector<std::shared_ptr<Player>>& players, std::shared_ptr<Player> curPlayer, Board& board) = 0;
};

#endif // CARD_HPP
