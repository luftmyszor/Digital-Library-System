#pragma once
#include <string>
#include <vector>
#include "Transaction.h"
#include "InventoryManager.h"

class TransactionManager
{
private:
    std::vector<Transaction> _transactions;
    std::string _transactionFile;
    InventoryManager &_inventory;

    long long getCurrentTimestamp() const;

public:
    struct OverdueRecord
    {
        std::string transactionId;
        std::string username;
        std::string bookId;
        int daysOverdue;
        double penaltyAmount;
    };

    TransactionManager(const std::string &filepath, InventoryManager &inventory);

    void loadTransactions();
    void saveTransactions() const;

    // Core Use Cases
    bool borrowBook(const std::string &username, const std::string &bookId);
    bool returnBook(const std::string &username, const std::string &bookId);
    bool reserveBook(const std::string &username, const std::string &bookId);

    std::vector<OverdueRecord> calculateOverduePenalties(double dailyRate) const;
};