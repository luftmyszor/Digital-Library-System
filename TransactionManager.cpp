#include "TransactionManager.h"
#include "DataManager.h"
#include <chrono>
#include <iostream>

TransactionManager::TransactionManager(const std::string &filepath, InventoryManager &inventory)
    : _transactionFile(filepath), _inventory(inventory)
{
    loadTransactions();
}

long long TransactionManager::getCurrentTimestamp() const
{
    auto now = std::chrono::system_clock::now().time_since_epoch();
    return std::chrono::duration_cast<std::chrono::seconds>(now).count();
}

void TransactionManager::loadTransactions()
{
    _transactions.clear();
    auto lines = DataManager::readLines(_transactionFile);
    for (const auto &line : lines)
    {
        if (!line.empty())
        {
            _transactions.push_back(Transaction::deserialize(line));
        }
    }
}

void TransactionManager::saveTransactions() const
{
    std::vector<std::string> lines;
    for (const auto &t : _transactions)
    {
        lines.push_back(t.serialize());
    }
    DataManager::writeLines(_transactionFile, lines);
}

bool TransactionManager::borrowBook(const std::string &username, const std::string &bookId)
{
    Book *book = _inventory.getBookById(bookId);

    // Check if book exists and is actually available
    if (book == nullptr || !book->getIsAvailable())
    {
        return false;
    }

    // 1. Update book state
    book->setAvailable(false);
    _inventory.saveInventory(); // Save changes to disk immediately

    // 2. Create transaction record
    Transaction t;
    t.transactionId = "TXN" + std::to_string(getCurrentTimestamp());
    t.username = username;
    t.bookId = bookId;
    t.borrowDate = getCurrentTimestamp();
    t.dueDate = t.borrowDate + (14 * 24 * 60 * 60); // 14 days in seconds
    t.returnDate = 0;
    t.status = TransStatus::BORROWED;

    _transactions.push_back(t);
    saveTransactions();
    return true;
}

bool TransactionManager::returnBook(const std::string &username, const std::string &bookId)
{
    // Find the active transaction
    for (auto &t : _transactions)
    {
        if (t.username == username && t.bookId == bookId && t.status == TransStatus::BORROWED)
        {

            // 1. Update transaction
            t.status = TransStatus::RETURNED;
            t.returnDate = getCurrentTimestamp();
            saveTransactions();

            // 2. Update book state
            Book *book = _inventory.getBookById(bookId);
            if (book != nullptr)
            {
                book->setAvailable(true);
                _inventory.saveInventory();
            }
            return true; // Successfully returned
        }
    }
    return false; // No active borrowing found for this user/book combo
}

bool TransactionManager::reserveBook(const std::string &username, const std::string &bookId)
{
    Book *book = _inventory.getBookById(bookId);
    if (book == nullptr || book->getIsAvailable())
    {
        return false; // Can only reserve books that exist and are currently checked out
    }

    Transaction t;
    t.transactionId = "RES" + std::to_string(getCurrentTimestamp());
    t.username = username;
    t.bookId = bookId;
    t.borrowDate = getCurrentTimestamp();
    t.dueDate = 0;
    t.returnDate = 0;
    t.status = TransStatus::RESERVED;

    _transactions.push_back(t);
    saveTransactions();
    return true;
}

std::vector<TransactionManager::OverdueRecord> TransactionManager::calculateOverduePenalties(double dailyRate) const
{
    std::vector<OverdueRecord> report;
    long long now = getCurrentTimestamp();
    const long long secondsPerDay = 24 * 60 * 60;

    for (const auto &t : _transactions)
    {
        if (t.status != TransStatus::BORROWED)
            continue;

        if (now <= t.dueDate)
            continue;

        long long overdueSeconds = now - t.dueDate;
        int daysOverdue = static_cast<int>((overdueSeconds + secondsPerDay - 1) / secondsPerDay);

        OverdueRecord record;
        record.transactionId = t.transactionId;
        record.username = t.username;
        record.bookId = t.bookId;
        record.daysOverdue = daysOverdue;
        record.penaltyAmount = daysOverdue * dailyRate;
        report.push_back(record);
    }

    return report;
}

std::vector<Transaction> TransactionManager::getUserHistory(const std::string &username) const
{
    std::vector<Transaction> history;
    for (const auto &t : _transactions)
    {
        if (t.username == username)
        {
            history.push_back(t);
        }
    }
    return history;
}