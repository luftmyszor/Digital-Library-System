#include "InventoryManager.h"
#include "DataManager.h"
#include <sstream>
#include <iostream>

InventoryManager::InventoryManager(const std::string &filepath) : _inventoryFile(filepath)
{
    loadInventory();
}

void InventoryManager::loadInventory()
{
    _inventory.clear();
    auto lines = DataManager::readLines(_inventoryFile);

    for (const auto &line : lines)
    {
        if (line.empty())
            continue;

        std::istringstream iss(line);
        std::string type, id, title, author, category, availStr;

        if (std::getline(iss, type, ';') &&
            std::getline(iss, id, ';') &&
            std::getline(iss, title, ';') &&
            std::getline(iss, author, ';') &&
            std::getline(iss, category, ';') &&
            std::getline(iss, availStr, ';'))
        {
            bool isAvail = (availStr == "1");

            if (type == "P")
            {
                std::string shelfLoc;
                std::getline(iss, shelfLoc, ';');
                _inventory[id] = std::make_unique<PhysicalBook>(id, title, author, category, isAvail, shelfLoc);
            }
            else if (type == "D")
            {
                std::string format, sizeStr;
                std::getline(iss, format, ';');
                std::getline(iss, sizeStr, ';');
                _inventory[id] = std::make_unique<DigitalBook>(id, title, author, category, isAvail, format, std::stod(sizeStr));
            }
        }
    }
}

void InventoryManager::saveInventory() const
{
    std::vector<std::string> lines;
    for (const auto &pair : _inventory)
    {
        lines.push_back(pair.second->serialize());
    }
    DataManager::writeLines(_inventoryFile, lines);
}

bool InventoryManager::addBook(std::unique_ptr<Book> newBook)
{
    if (_inventory.find(newBook->getId()) != _inventory.end())
        return false; // ID already exists

    _inventory[newBook->getId()] = std::move(newBook);
    saveInventory(); // Save changes via DataManager
    return true;
}

bool InventoryManager::removeBook(const std::string &bookId)
{
    if (_inventory.erase(bookId) > 0)
    {
        saveInventory();
        return true;
    }
    return false;
}

Book *InventoryManager::getBookById(const std::string &bookId) const
{
    auto it = _inventory.find(bookId);
    if (it != _inventory.end())
    {
        return it->second.get();
    }
    return nullptr;
}

// Basic search functionality for the future TUI integration
std::vector<Book *> InventoryManager::searchByTitle(const std::string &titleQuery) const
{
    std::vector<Book *> results;
    for (const auto &pair : _inventory)
    {
        // Simple substring search
        if (pair.second->getTitle().find(titleQuery) != std::string::npos)
        {
            results.push_back(pair.second.get());
        }
    }
    return results;
}

std::vector<Book *> InventoryManager::getAllBooks() const
{
    std::vector<Book *> results;
    for (const auto &pair : _inventory)
    {
        results.push_back(pair.second.get());
    }
    return results;
}