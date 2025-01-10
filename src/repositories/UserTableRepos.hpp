#pragma once

#include "../models/User.hpp"
#include <drogon/HttpAppFramework.h>

class UserRepos {
    using id = size_t;
public:
    UserRepos() : dbClient_(drogon::app().getDbClient("default")) {}

public:
    void create_user(const User &);

    void delete_user(const User &);

    User read_user(id);

    void update_user(const User &);

    std::string find_user_hashPassword(const std::string &username, const std::string &email);

private:
    drogon::orm::DbClientPtr dbClient_;
};
