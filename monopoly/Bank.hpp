﻿#ifndef BANK_HPP
#define BANK_HPP

class Bank {
private:
    long long totalMoneyInBank; // Total money in the bank
    static Bank* instance;      // Singleton instance pointer

    // Private constructor to prevent external instantiation
    Bank();

public:
    // Get the Singleton instance
    static Bank* getInstance();

    // Bank money operations
    void payToPlayer(long long amount);
    void receiveFromPlayer(long long amount);
    long long getTotalMoneyInBank() const;
};

#endif // BANK_HPP
