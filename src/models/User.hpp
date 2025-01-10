#pragma once
#include <string>

class User {
public:
    User(std::string username, std::string hashPassword, std::string email) :
            hashPassword_(hashPassword),
            username_(username),
            email_(email) {}

    ~User() = default;

    std::string getUsername() const { return username_; }

    std::string getHashPassword_() const { return hashPassword_; }

    std::string getEmail() const { return email_; }

protected:
    std::string hashPassword_;
    std::string username_;
    std::string email_;
};