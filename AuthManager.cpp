#include "AuthManager.h"
#include "DataManager.h"
#include <sstream>

AuthManager::AuthManager(const std::string &filepath) : _accountsFile(filepath)
{
    loadUsers();
}

std::string AuthManager::hashPassword(const std::string &str)
{
    const unsigned long long p = 31;
    const unsigned long long m = 1e9 + 9;
    unsigned long long hash_value = 0;
    unsigned long long p_pow = 1;

    for (char c : str)
    {
        hash_value = (hash_value + (c - 'a' + 1) * p_pow) % m;
        p_pow = (p_pow * p) % m;
    }

    std::stringstream ss;
    ss << std::hex << hash_value;
    return ss.str();
}

void AuthManager::loadUsers()
{
    _users.clear();
    auto lines = DataManager::readLines(_accountsFile);

    for (const auto &line : lines)
    {
        if (line.empty())
            continue;

        std::istringstream iss(line);
        std::string roleStr, username, passHash;

        // role;username;passwordHash
        if (std::getline(iss, roleStr, ';') &&
            std::getline(iss, username, ';') &&
            std::getline(iss, passHash, ';'))
        {

            Account acc;
            acc.role = static_cast<UserRole>(std::stoi(roleStr));
            acc.username = username;
            acc.passwordHash = passHash;
            _users[username] = acc;
        }
    }

    // Create a default admin if the system is completely empty
    if (_users.empty())
    {
        signUp("admin", "admin", ADMIN);
    }
}

void AuthManager::saveUsers()
{
    std::vector<std::string> lines;
    for (const auto &pair : _users)
    {
        const Account &acc = pair.second;
        std::ostringstream oss;
        oss << static_cast<int>(acc.role) << ";" << acc.username << ";" << acc.passwordHash;
        lines.push_back(oss.str());
    }
    DataManager::writeLines(_accountsFile, lines);
}

bool AuthManager::signUp(const std::string &username, const std::string &password, UserRole role)
{
    if (doesUserExist(username))
        return false; // Username taken

    Account acc;
    acc.role = role;
    acc.username = username;
    acc.passwordHash = hashPassword(password);

    _users[username] = acc;
    saveUsers(); // Save changes to file immediately via DataManager
    return true;
}

bool AuthManager::signIn(const std::string &username, const std::string &password, Account &outAccount)
{
    if (!doesUserExist(username))
        return false;

    Account &acc = _users[username];

    if (acc.passwordHash == hashPassword(password))
    {
        outAccount = acc;
        return true;
    }
    return false;
}

bool AuthManager::doesUserExist(const std::string &username)
{
    return _users.find(username) != _users.end();
}