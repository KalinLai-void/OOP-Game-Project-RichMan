#ifndef CARD_HPP
#define CARD_HPP

#include <string>

class Card {
private:
    std::string name;
    int price;
    std::string effect; // Card effect

public:
    Card(const std::string& n, int p, const std::string& e)
        : name(n), price(p), effect(e) {}

    std::string getName() const {
        return name;
    }
    int getPrice() const {
        return price;
    }
    std::string getEffect() const {
        return effect;
    }
};

#endif // CARD_HPP
