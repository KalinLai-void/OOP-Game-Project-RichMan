#ifndef BANK_HPP
#define BANK_HPP
#include "Game/Player.hpp"
class Bank {
private:
    long long totalMoneyInBank; // Total money in the bank
    static Bank* instance;      // Singleton instance pointer

    // Private constructor to prevent external instantiation
    Bank();

public:
    // Get the Singleton instance
    static Bank* getInstance();
    static void destroyInstance();

    // Bank money operations
    void payToPlayer(std::shared_ptr<Player> player, long long amount);
    void receiveFromPlayer(std::shared_ptr<Player> player, long long amount);
    long long getTotalMoneyInBank() const;
};

#endif // BANK_HPP
