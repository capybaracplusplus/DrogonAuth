#pragma once

#include <string>

class UserDto {
public:
    UserDto(std::string username, std::string password, std::string email) :
            password_(password),
            username_(username),
            email_(email) {}

    ~UserDto() = default;

    std::string getUsername() const { return username_; }

    std::string getPassword_() const { return password_; }

    std::string getEmail() const { return email_; }

protected:
    std::string password_;
    std::string username_;
    std::string email_;
};