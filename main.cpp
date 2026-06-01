#include <iostream>
#include <memory>
#include "DecisionNode.h"
#include "TreeManager.h"

int main()
{
    // 1. Create the root of the Digital Library System
    auto root = std::make_unique<DecisionNode>("Digital Library System");
    DecisionNode *libraryRoot = root.get();

    // ==========================================
    // BRANCH 1: READER PORTAL
    // ==========================================
    DecisionNode *readerMenu = libraryRoot->addChild("Reader Portal", "Enter as Reader");

    readerMenu->addChild("User Registration", "Register New Account", []()
                         { std::cout << "\n>>> [USE CASE 1] Launching Reader Registration Wizard...\n"; });
    readerMenu->addChild("Search Books", "Search & Filter Books", []()
                         { std::cout << "\n>>> [USE CASE 2] Opening Book Catalog Search (Title/Author/Category)...\n"; });
    readerMenu->addChild("Borrow Book", "Borrow a Book (Physical/Digital)", []()
                         { std::cout << "\n>>> [USE CASE 3] Checking availability and updating borrowing history...\n"; });
    readerMenu->addChild("Return Book", "Return a Borrowed Book", []()
                         { std::cout << "\n>>> [USE CASE 4] Processing return and checking for overdue status...\n"; });
    readerMenu->addChild("Reserve Book", "Reserve Unavailable Book", []()
                         { std::cout << "\n>>> [USE CASE 5] Book placed on hold. You will be notified when available...\n"; });

    // ==========================================
    // BRANCH 2: LIBRARIAN PORTAL
    // ==========================================
    DecisionNode *librarianMenu = libraryRoot->addChild("Librarian Portal", "Enter as Librarian");

    librarianMenu->addChild("Search Books", "Catalog Search", []()
                            { std::cout << "\n>>> [USE CASE 2] Opening Book Catalog Search...\n"; });
    librarianMenu->addChild("Calculate Penalties", "Calculate Overdue Penalties", []()
                            { std::cout << "\n>>> [USE CASE 6] Scanning database for overdue books and generating fees...\n"; });
    librarianMenu->addChild("Add Book", "Add New Book to Inventory", []()
                            { std::cout << "\n>>> [USE CASE 7] Enter book metadata (Physical/E-Book)... Item added to stock.\n"; });
    librarianMenu->addChild("Remove Book", "Remove Book from Inventory", []()
                            { std::cout << "\n>>> [USE CASE 8] Removing damaged or decommissioned asset from system...\n"; });

    // ==========================================
    // BRANCH 3: ADMINISTRATOR PORTAL
    // ==========================================
    DecisionNode *adminMenu = libraryRoot->addChild("Administrator Portal", "Enter as Administrator");

    adminMenu->addChild("User Management", "Manage User Registration/Auth", []()
                        { std::cout << "\n>>> [USE CASE 1] Opening credential policies and user override tools...\n"; });
    adminMenu->addChild("Inventory Audit - Add", "Direct Add Asset", []()
                        { std::cout << "\n>>> [USE CASE 7] Administrative inventory bypass: Adding asset...\n"; });
    adminMenu->addChild("Inventory Audit - Remove", "Direct Remove Asset", []()
                        { std::cout << "\n>>> [USE CASE 8] Administrative inventory bypass: Removing asset...\n"; });

    // ==========================================
    // KICKSTART THE TUI
    // ==========================================
    TreeManager tui(std::move(root));
    tui.start();

    return 0;
}