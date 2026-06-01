#include <iostream>
#include <memory>
#include <string>
#include "DecisionNode.h"
#include "TreeManager.h"
#include "AuthManager.h"

int main()
{
    AuthManager auth("accounts.txt");
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
        // TUI TREE SETUP (Filtered by Role)
        // ==========================================
        if (activeUser.role == READER)
        {
            DecisionNode *readerMenu = libraryRoot->addChild("Reader Portal", "Enter as Reader");

            readerMenu->addChild("Search Books", "Search & Filter Books", []()
                                 { std::cout << "\n>>> [USE CASE 2] Opening Book Catalog Search...\n"; });
            readerMenu->addChild("Borrow Book", "Borrow a Book (Physical/Digital)", []()
                                 { std::cout << "\n>>> [USE CASE 3] Checking availability and updating history...\n"; });
            readerMenu->addChild("Return Book", "Return a Borrowed Book", []()
                                 { std::cout << "\n>>> [USE CASE 4] Processing return...\n"; });
            readerMenu->addChild("Reserve Book", "Reserve Unavailable Book", []()
                                 { std::cout << "\n>>> [USE CASE 5] Book placed on hold...\n"; });
        }
        else if (activeUser.role == LIBRARIAN)
        {
            DecisionNode *librarianMenu = libraryRoot->addChild("Librarian Portal", "Enter as Librarian");

            librarianMenu->addChild("Search Books", "Catalog Search", []()
                                    { std::cout << "\n>>> [USE CASE 2] Opening Book Catalog Search...\n"; });
            librarianMenu->addChild("Calculate Penalties", "Calculate Overdue Penalties", []()
                                    { std::cout << "\n>>> [USE CASE 6] Scanning database for overdue books...\n"; });
            librarianMenu->addChild("Add Book", "Add New Book to Inventory", []()
                                    { std::cout << "\n>>> [USE CASE 7] Enter book metadata... Item added.\n"; });
            librarianMenu->addChild("Remove Book", "Remove Book from Inventory", []()
                                    { std::cout << "\n>>> [USE CASE 8] Removing damaged asset...\n"; });
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