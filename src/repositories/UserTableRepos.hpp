#pragma once

#include "../models/User.hpp"
#include <drogon/HttpAppFramework.h>
#include "string"

class UserRepos {
    using id = size_t;
    using DbClientType = drogon::orm::DbClientPtr;
    struct UserAuth {
        id id_;
        std::string password;
    };
public:
    UserRepos() : dbClient_(drogon::app().getDbClient("default")) {}

public:
    void create_user(const User &);

    void delete_user(const User &);

    User read_user(id);

    void update_user(const User &);

    UserAuth getUserAuthData(const std::string &username, const std::string &email);

private:
    DbClientType dbClient_;
};
