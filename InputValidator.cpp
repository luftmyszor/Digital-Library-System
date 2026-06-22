#include "InputValidator.h"

double InputValidator::getDouble(const std::string &prompt)
{
    double value;
    while (true)
    {
        std::cout << prompt;
        if (std::cin >> value)
            return value;
        std::cout << "[!] Invalid input. Please enter a number.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

int InputValidator::getInt(const std::string &prompt)
{
    int value;
    while (true)
    {
        std::cout << prompt;
        if (std::cin >> value)
            return value;
        std::cout << "[!] Invalid input. Please enter an integer.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

std::string InputValidator::getString(const std::string &prompt)
{
    std::string value;
    std::cout << prompt;
    std::cin >> std::ws;
    std::getline(std::cin, value);
    return value;
}