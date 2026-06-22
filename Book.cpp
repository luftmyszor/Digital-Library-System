#include "Book.h"
#include <iostream>
#include <sstream>

// --- Base Book Implementation ---
Book::Book(const std::string &id, const std::string &title, const std::string &author, const std::string &category, bool isAvailable)
    : id(id), title(title), author(author), category(category), isAvailable(isAvailable) {}

std::string Book::getId() const { return id; }
std::string Book::getTitle() const { return title; }
std::string Book::getAuthor() const { return author; }
std::string Book::getCategory() const { return category; }
bool Book::getIsAvailable() const { return isAvailable; }
void Book::setAvailable(bool status) { isAvailable = status; }

// --- Physical Book Implementation ---
PhysicalBook::PhysicalBook(const std::string &id, const std::string &title, const std::string &author,
                           const std::string &category, bool isAvailable, const std::string &shelfLocation)
    : Book(id, title, author, category, isAvailable), shelfLocation(shelfLocation) {}

std::string PhysicalBook::serialize() const
{
    std::ostringstream oss;
    // Format: P;ID;Title;Author;Category;IsAvailable;ShelfLocation
    oss << "P;" << id << ";" << title << ";" << author << ";" << category << ";" << isAvailable << ";" << shelfLocation;
    return oss.str();
}

void PhysicalBook::displayInfo() const
{
    std::cout << "[ID: " << id << "] [Physical] " << title << " by " << author << " | Cat: " << category
              << " | Loc: " << shelfLocation << " | Status: " << (isAvailable ? "Available" : "Borrowed") << "\n";
}

// --- Digital Book Implementation ---
DigitalBook::DigitalBook(const std::string &id, const std::string &title, const std::string &author,
                         const std::string &category, bool isAvailable, const std::string &fileFormat, double fileSizeMB)
    : Book(id, title, author, category, isAvailable), fileFormat(fileFormat), fileSizeMB(fileSizeMB) {}

std::string DigitalBook::serialize() const
{
    std::ostringstream oss;
    // Format: D;ID;Title;Author;Category;IsAvailable;Format;Size
    oss << "D;" << id << ";" << title << ";" << author << ";" << category << ";" << isAvailable << ";" << fileFormat << ";" << fileSizeMB;
    return oss.str();
}

void DigitalBook::displayInfo() const
{
    std::cout << "[ID: " << id << "] [Digital] " << title << " by " << author << " | Cat: " << category
              << " | Format: " << fileFormat << " (" << fileSizeMB << "MB) | Status: " << (isAvailable ? "Available" : "Borrowed") << "\n";
}