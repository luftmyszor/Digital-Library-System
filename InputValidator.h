#pragma once
#include <string>
#include <limits>
#include <iostream>

class InputValidator
{
public:
    static double getDouble(const std::string &prompt);
    static int getInt(const std::string &prompt);
    static std::string getString(const std::string &prompt);
};