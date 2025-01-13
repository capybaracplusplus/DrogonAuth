#pragma once

#include "../models/user.hpp"
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
    void create_user(const user &);

    void delete_user(const user &);

    user read_user(id);

    void update_user(const user &);

    UserAuth getUserAuthData(const std::string &username, const std::string &email);

private:
    DbClientType dbClient_;
};
