#include "TreeManager.h"
#include <iostream>
#include <string>
#include <limits>

TreeManager::TreeManager(std::unique_ptr<DecisionNode> rootNode)
    : root(std::move(rootNode)), isRunning(false)
{
    currentNode = root.get();
}

void TreeManager::start()
{
    isRunning = true;

    currentNode->executePayload();

    while (isRunning)
    {
        currentNode->displayMenu();
        handleInput();
    }

    std::cout << "\nExiting TUI...\n";
}

void TreeManager::stop()
{
    isRunning = false;
}

void TreeManager::handleInput()
{
    std::string input;
    std::cin >> input;

    if (input == "Q" || input == "q")
    {
        stop();
        return;
    }

    try
    {
        int choice = std::stoi(input);
        const auto &children = currentNode->getChildren();

        if (choice == 0)
        {
            if (currentNode->getParent() != nullptr)
            {
                currentNode = currentNode->getParent();
                currentNode->executePayload();
            }
            else
            {
                std::cout << "\n[!] Already at the top level menu. Cannot go back.\n";
            }
        }
        else if (choice > 0 && choice <= static_cast<int>(children.size()))
        {
            currentNode = children[choice - 1].get(); // Move navigation down
            currentNode->executePayload();            // Trigger payload
        }
        // Out of bounds
        else
        {
            std::cout << "\n[!] Invalid option. Please select a valid number.\n";
        }
    }
    catch (const std::invalid_argument &)
    {
        std::cout << "\n[!] Invalid input. Please enter a number, or 'Q' to quit.\n";
    }

    // Clear any extra characters left in the input buffer
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}