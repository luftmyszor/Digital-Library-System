#include "Transaction.h"
#include <sstream>

std::string Transaction::serialize() const
{
    std::ostringstream oss;
    oss << transactionId << ";" << username << ";" << bookId << ";"
        << borrowDate << ";" << dueDate << ";" << returnDate << ";"
        << static_cast<int>(status);
    return oss.str();
}

Transaction Transaction::deserialize(const std::string &line)
{
    Transaction t;
    std::istringstream iss(line);
    std::string statusStr, bDate, dDate, rDate;

    std::getline(iss, t.transactionId, ';');
    std::getline(iss, t.username, ';');
    std::getline(iss, t.bookId, ';');
    std::getline(iss, bDate, ';');
    std::getline(iss, dDate, ';');
    std::getline(iss, rDate, ';');
    std::getline(iss, statusStr, ';');

    t.borrowDate = std::stoll(bDate);
    t.dueDate = std::stoll(dDate);
    t.returnDate = std::stoll(rDate);
    t.status = static_cast<TransStatus>(std::stoi(statusStr));

    return t;
}