#ifndef BANK_HPP
#define BANK_HPP

class Bank {
private:
    long long totalMoneyInBank; // 銀行總資金
    static Bank* instance;      // Singleton 實例指標

    // 私有建構子，禁止外部建立
    Bank();

public:
    // 取得 Singleton 實例
    static Bank* getInstance();

    // 操作銀行資金
    void payToPlayer(long long amount);
    void receiveFromPlayer(long long amount);
    long long getTotalMoneyInBank() const;
};

#endif // BANK_HPP
