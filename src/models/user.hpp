#pragma once
#include <string>

class user {
public:
    user(const std::string& username, const std::string& hashPassword, const std::string& email) :
            hashPassword_(hashPassword),
            username_(username),
            email_(email) {}

    ~user() = default;

    const std::string& getUsername() const { return username_; }

    const std::string& getHashPassword_() const { return hashPassword_; }

    const std::string& getEmail() const { return email_; }

protected:
    std::string hashPassword_;
    std::string username_;
    std::string email_;
};