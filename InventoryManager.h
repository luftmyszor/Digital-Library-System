#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include <vector>
#include "Book.h"

class InventoryManager
{
private:
    std::unordered_map<std::string, std::unique_ptr<Book>> _inventory;
    std::string _inventoryFile;

public:
    InventoryManager(const std::string &filepath);

    void loadInventory();
    void saveInventory() const;

    // Use Case 7: Add Book
    bool addBook(std::unique_ptr<Book> newBook);

    // Use Case 8: Remove Book
    bool removeBook(const std::string &bookId);
    bool updateBook(const std::string &bookId,
                    const std::string &newTitle,
                    const std::string &newAuthor,
                    const std::string &newCategory,
                    bool newAvailability);

    // Use Case 2: Search functionality (returning raw pointers for safe viewing without taking ownership)
    std::vector<Book *> searchByTitle(const std::string &titleQuery) const;
    std::vector<Book *> filterByAuthor(const std::string &authorQuery) const;
    std::vector<Book *> filterByCategory(const std::string &category) const;
    std::vector<Book *> getAllBooks() const;

    // Lookups for transactions
    Book *getBookById(const std::string &bookId) const;
};