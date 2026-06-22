#include <iostream>
#include <memory>
#include <string>
#include <chrono>
#include "DecisionNode.h"
#include "TreeManager.h"
#include "AuthManager.h"
#include "InventoryManager.h"
#include "Book.h"
#include "TransactionManager.h"
#include "InputValidator.h"

int main()
{
    AuthManager auth("accounts.txt");
    InventoryManager inventory("books.txt");
    TransactionManager transactions("transactions.txt", inventory);
    bool appRunning = true;

    while (appRunning)
    {
        Account activeUser;
        bool loggedIn = false;

        // ==========================================
        // 0. LOGIN WALL & PUBLIC REGISTRATION
        // ==========================================
        while (!loggedIn)
        {
            std::cout << "\n========================================\n";
            std::cout << "       Digital Library System           \n";
            std::cout << "========================================\n";
            std::cout << "  [1] Sign In\n";
            std::cout << "  [2] Sign Up (New Reader Account)\n";
            std::cout << "  [3] Exit Application\n";
            std::cout << "----------------------------------------\n";
            std::cout << "Select an option: ";

            std::string choice;
            std::cin >> choice;

            if (choice == "1")
            {
                std::string user, pass;
                std::cout << "Username: ";
                std::cin >> user;
                std::cout << "Password: ";
                std::cin >> pass;

                if (auth.signIn(user, pass, activeUser))
                {
                    std::cout << "\n>>> Access Granted. Welcome, " << activeUser.username << "!\n";
                    loggedIn = true;
                }
                else
                {
                    std::cout << "\n[!] Invalid credentials. Try again.\n";
                }
            }
            else if (choice == "2")
            {
                std::string newU, newP;
                std::cout << "Choose a username: ";
                std::cin >> newU;
                std::cout << "Choose a password: ";
                std::cin >> newP;

                // Public signups default to the READER role (0)
                if (auth.signUp(newU, newP, READER))
                {
                    std::cout << "\n>>> Account created successfully! You can now sign in.\n";
                }
                else
                {
                    std::cout << "\n[!] Username already exists. Please choose another.\n";
                }
            }
            else if (choice == "3" || choice == "q" || choice == "Q")
            {
                std::cout << "\nShutting down Digital Library System...\n";
                return 0; // Completely exit the application
            }
            else
            {
                std::cout << "\n[!] Invalid option. Please select 1, 2, or 3.\n";
            }
        }

        // ==========================================
        // 1. Create the root of the Digital Library System
        // ==========================================
        auto root = std::make_unique<DecisionNode>("Digital Library Dashboard (" + activeUser.username + ")");
        DecisionNode *libraryRoot = root.get();

        // ==========================================
        // SHARED LAMBDAS
        // ==========================================
        auto searchCatalogAction = [&inventory]()
        {
            std::cout << "\n>>> [USE CASE 2] Book Catalog Search\n";
            std::cout << "Search mode: [1] Title  [2] Author  [3] Category  [4] List All\n";
            std::cout << "Select mode: ";
            std::string mode;
            std::cin >> mode;

            if (mode == "4")
            {
                auto allBooks = inventory.getAllBooks();
                std::cout << "\n--- Search Results ---\n";
                if (allBooks.empty())
                {
                    std::cout << "[!] No books found.\n";
                }
                else
                {
                    for (const auto *book : allBooks)
                    {
                        book->displayInfo();
                    }
                }
                std::cout << "----------------------\n";
                return;
            }

            std::cout << "Enter search text: ";
            std::string query;
            std::cin >> std::ws; // Clear whitespace buffer
            std::getline(std::cin, query);

            std::vector<Book *> results;
            if (mode == "1")
            {
                results = inventory.searchByTitle(query);
            }
            else if (mode == "2")
            {
                results = inventory.filterByAuthor(query);
            }
            else if (mode == "3")
            {
                results = inventory.filterByCategory(query);
            }
            else
            {
                std::cout << "[!] Invalid search mode.\n";
                return;
            }

            std::cout << "\n--- Search Results ---\n";
            if (results.empty())
            {
                std::cout << "[!] No books found.\n";
            }
            else
            {
                for (const auto *book : results)
                {
                    book->displayInfo();
                }
            }
            std::cout << "----------------------\n";
        };

        // ==========================================
        // TUI TREE SETUP (Filtered by Role)
        // ==========================================
        if (activeUser.role == READER)
        {
            DecisionNode *readerMenu = libraryRoot->addChild("Reader Portal", "Enter as Reader");

            readerMenu->addChild("Search Books", "Search & Filter Books", searchCatalogAction);

            readerMenu->addChild("Borrow Book", "Borrow a Book", [&transactions, &activeUser]()
                                 {
                std::cout << "\n>>> [USE CASE 3] Borrow a Book\n";
                std::cout << "Enter the Book ID you wish to borrow: ";
                std::string bId; std::cin >> bId;
                
                if (transactions.borrowBook(activeUser.username, bId)) {
                    std::cout << ">>> Success! Enjoy your book.\n";
                } else {
                    std::cout << "[!] Error: Book is either unavailable or does not exist.\n";
                } });

            readerMenu->addChild("Return Book", "Return a Borrowed Book", [&transactions, &activeUser]()
                                 {
                std::cout << "\n>>> [USE CASE 4] Return a Book\n";
                std::cout << "Enter the Book ID you wish to return: ";
                std::string bId; std::cin >> bId;
                
                if (transactions.returnBook(activeUser.username, bId)) {
                    std::cout << ">>> Success! The book has been returned.\n";
                } else {
                    std::cout << "[!] Error: You do not currently have this book checked out.\n";
                } });

            readerMenu->addChild("Reserve Book", "Reserve Unavailable Book", [&transactions, &activeUser]()
                                 {
                std::cout << "\n>>> [USE CASE 5] Reserve a Book\n";
                std::cout << "Enter the Book ID you wish to reserve: ";
                std::string bId; std::cin >> bId;

                if (transactions.reserveBook(activeUser.username, bId)) {
                    std::cout << ">>> Success! You have been added to the reservation queue.\n";
                } else {
                    std::cout << "[!] Error: Book does not exist or is currently available to borrow right now.\n";
                } });
        }
        else if (activeUser.role == LIBRARIAN)
        {
            DecisionNode *librarianMenu = libraryRoot->addChild("Librarian Portal", "Enter as Librarian");

            librarianMenu->addChild("Search Books", "Catalog Search", searchCatalogAction);
            librarianMenu->addChild("Calculate Penalties", "Calculate Overdue Penalties", [&transactions]()
                                    { 
                std::cout << "\n>>> [USE CASE 6] Scanning database for overdue books...\n";
                
                double dailyRate = 2.50; // Set the fine to $2.50 per day
                auto report = transactions.calculateOverduePenalties(dailyRate);

                if (report.empty()) {
                    std::cout << ">>> All clear! No overdue books found.\n";
                } else {
                    std::cout << "--- OVERDUE PENALTY REPORT ---\n";
                    for (const auto& record : report) {
                        std::cout << "[User: " << record.username 
                                  << "] Book ID: " << record.bookId 
                                  << " | Days Overdue: " << record.daysOverdue 
                                  << " | Fine: $" << record.penaltyAmount << "\n";
                    }
                    std::cout << "------------------------------\n";
                } });

            // Add Book Logic
            librarianMenu->addChild("Add Book", "Add New Book to Inventory", [&inventory]()
                                    { 
                std::cout << "\n>>> [USE CASE 7] Add New Asset\n";
                
                std::string title = InputValidator::getString("Title: ");
                std::string author = InputValidator::getString("Author: ");
                std::string category = InputValidator::getString("Category: ");
                std::string isDigital = InputValidator::getString("Is this a digital format? (y/n): ");

                // Generate a unique ID using modern C++ chrono
                auto now = std::chrono::system_clock::now().time_since_epoch();
                long long timestamp = std::chrono::duration_cast<std::chrono::seconds>(now).count();
                std::string id = "B" + std::to_string(timestamp);

                if (isDigital == "y" || isDigital == "Y") {
                    std::string format;
                    std::cout << "File Format (e.g., PDF, EPUB): "; std::cin >> format;
                    double size = InputValidator::getDouble("File Size (MB): ");
                    
                    inventory.addBook(std::make_unique<DigitalBook>(id, title, author, category, true, format, size));
                } else {
                    std::string shelf;
                    std::cout << "Shelf Location (e.g., A1, B2): "; std::cin >> std::ws; std::getline(std::cin, shelf);
                    
                    inventory.addBook(std::make_unique<PhysicalBook>(id, title, author, category, true, shelf));
                }
                std::cout << ">>> Asset added successfully! Unique ID: " << id << "\n"; });

            // Remove Book Logic
            librarianMenu->addChild("Remove Book", "Remove Book from Inventory", [&inventory]()
                                    { 
                std::cout << "\n>>> [USE CASE 8] Remove Asset\n";
                std::cout << "Enter the Book ID to remove: ";
                std::string id;
                std::cin >> id;
                
                if (inventory.removeBook(id)) {
                    std::cout << ">>> Asset " << id << " successfully removed.\n";
                } else {
                    std::cout << "[!] Error: Book ID not found.\n";
                } });
        }
        else if (activeUser.role == ADMIN)
        {
            DecisionNode *adminMenu = libraryRoot->addChild("Administrator Portal", "Enter as Administrator");

            adminMenu->addChild("User Management", "Create High-Level Users", [&auth]()
                                { 
                                    std::cout << "\n>>> [USE CASE 1] Registering a new system user...\n";
                                    std::cout << "Enter new username: ";
                                    std::string newU; std::cin >> newU;
                                    std::cout << "Enter new password: ";
                                    std::string newP; std::cin >> newP;
                                    std::cout << "Role (0=Reader, 1=Librarian, 2=Admin): ";
                                    int roleInt; std::cin >> roleInt;
                                    
                                    if(auth.signUp(newU, newP, static_cast<UserRole>(roleInt))) {
                                        std::cout << ">>> User successfully created!\n";
                                    } else {
                                        std::cout << ">>> [!] Username already exists.\n";
                                    } });

            // Note: In the future, you could easily point these admin placeholders to the inventory manager too!
            adminMenu->addChild("Inventory Audit - Add", "Direct Add Asset", []()
                                { std::cout << "\n>>> [USE CASE 7] Administrative inventory bypass: Adding asset...\n"; });
            adminMenu->addChild("Inventory Audit - Remove", "Direct Remove Asset", []()
                                { std::cout << "\n>>> [USE CASE 8] Administrative inventory bypass: Removing asset...\n"; });
        }

        // ==========================================
        // START THE TUI
        // ==========================================
        TreeManager tui(std::move(root));
        tui.start();

        std::cout << "\n>>> Logging out of account: " << activeUser.username << "...\n";
    }

    return 0;
}