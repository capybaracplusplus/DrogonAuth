#pragma once

#include <string>

class UserBase {
public:
    virtual ~UserBase() = default;

    virtual void save() = 0;

    virtual void load(size_t userId) = 0;

    size_t getId() const { return id_; }

    std::string getUsername() const { return username_; }

    std::string getHashPassword_() const { return hashPassword_; }

protected:
    size_t id_;
    std::string hashPassword_;
    std::string username_;
};