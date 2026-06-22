#pragma once
#include <string>

// Base class representing common library asset data
class Book
{
protected:
    std::string id;
    std::string title;
    std::string author;
    std::string category;
    bool isAvailable;

public:
    Book(const std::string &id, const std::string &title, const std::string &author, const std::string &category, bool isAvailable);
    virtual ~Book() = default;

    // Getters
    std::string getId() const;
    std::string getTitle() const;
    std::string getAuthor() const;
    std::string getCategory() const;
    bool getIsAvailable() const;

    // Setters
    void setTitle(const std::string &newTitle);
    void setAuthor(const std::string &newAuthor);
    void setCategory(const std::string &newCategory);
    void setAvailable(bool status);

    // Pure virtual functions for polymorphism
    virtual std::string getType() const = 0;
    virtual std::string serialize() const = 0;
    virtual void displayInfo() const = 0;
};

// Derived class for Physical Books
class PhysicalBook : public Book
{
private:
    std::string shelfLocation;

public:
    PhysicalBook(const std::string &id, const std::string &title, const std::string &author,
                 const std::string &category, bool isAvailable, const std::string &shelfLocation);

    std::string getType() const override { return "PHYSICAL"; }
    std::string serialize() const override;
    void displayInfo() const override;
};

// Derived class for Digital Books (e-books)
class DigitalBook : public Book
{
private:
    std::string fileFormat; // e.g., PDF, EPUB
    double fileSizeMB;

public:
    DigitalBook(const std::string &id, const std::string &title, const std::string &author,
                const std::string &category, bool isAvailable, const std::string &fileFormat, double fileSizeMB);

    std::string getType() const override { return "DIGITAL"; }
    std::string serialize() const override;
    void displayInfo() const override;
};