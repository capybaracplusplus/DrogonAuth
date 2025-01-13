#pragma once

#include <string>

class UserDto {
public:
    UserDto(const std::string &username, const std::string &password, const std::string &email) :
            password_(password),
            username_(username),
            email_(email) {}

    ~UserDto() = default;

    const std::string &getUsername() const { return username_; }

    const std::string &getPassword_() const { return password_; }

    const std::string &getEmail() const { return email_; }

protected:
    std::string password_;
    std::string username_;
    std::string email_;
};