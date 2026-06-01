#pragma once
#include <string>
#include <unordered_map>

enum UserRole
{
    READER = 0,
    LIBRARIAN = 1,
    ADMIN = 2
};

struct Account
{
    UserRole role;
    std::string username;
    std::string passwordHash;
};

class AuthManager
{
private:
    std::unordered_map<std::string, Account> _users;
    std::string _accountsFile;

    std::string hashPassword(const std::string &plaintext);

public:
    AuthManager(const std::string &filepath);

    void loadUsers();
    void saveUsers();

    bool signUp(const std::string &username, const std::string &password, UserRole role);
    bool signIn(const std::string &username, const std::string &password, Account &outAccount);
    bool doesUserExist(const std::string &username);
};