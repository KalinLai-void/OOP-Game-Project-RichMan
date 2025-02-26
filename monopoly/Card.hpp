#ifndef CARD_HPP
#define CARD_HPP

#include <string>
#include <memory>

class Player; // Forward declaration to prevent circular dependency

class Card {
private:
    std::string name;
    int price;
    std::string effect; // Card effect

public:
    Card(const std::string& n, int p, const std::string& e)
        : name(n), price(p), effect(e) {}

    virtual ~Card() {};

    std::string getName() const {
        return name;
    }
    int getPrice() const {
        return price;
    }
    std::string getEffect() const {
        return effect;
    }

    virtual void useEffect(std::shared_ptr<Player> player) = 0;
};

#endif // CARD_HPP
