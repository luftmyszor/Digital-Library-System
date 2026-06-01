#include "DataManager.h"
#include <fstream>
#include <stdexcept>

std::vector<std::string> DataManager::readLines(const std::string &filename)
{
    std::vector<std::string> lines;
    std::ifstream ifs(filename);

    // If the file doesn't exist yet, just return an empty vector
    if (!ifs.is_open())
        return lines;

    std::string line;
    while (std::getline(ifs, line))
    {
        lines.push_back(line);
    }
    return lines;
}

void DataManager::writeLines(const std::string &filename, const std::vector<std::string> &lines)
{
    std::ofstream ofs(filename);

    if (!ofs.is_open())
    {
        throw std::runtime_error("Could not open file for writing: " + filename);
    }

    for (const auto &line : lines)
    {
        ofs << line << "\n";
    }
}