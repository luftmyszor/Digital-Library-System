#pragma once
#include <string>

enum class TransStatus
{
    BORROWED = 0,
    RETURNED = 1,
    RESERVED = 2
};

struct Transaction
{
    std::string transactionId;
    std::string username;
    std::string bookId;
    long long borrowDate; // Unix timestamp
    long long dueDate;    // Unix timestamp
    long long returnDate; // Unix timestamp (0 if not returned yet)
    TransStatus status;

    std::string serialize() const;
    static Transaction deserialize(const std::string &line);
};