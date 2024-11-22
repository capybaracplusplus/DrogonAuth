#pragma once

#include "UserBase.hpp"


class User : public UserBase {
public:
    ~User() override = default;

    void save() override;

    void load(size_t userId) override;

    std::string getEmail() const { return email_; }

private:
    std::string email_;
};


