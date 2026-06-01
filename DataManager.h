#pragma once
#include <string>
#include <vector>

class DataManager
{
public:
    static std::vector<std::string> readLines(const std::string &filename);

    static void writeLines(const std::string &filename, const std::vector<std::string> &lines);
};