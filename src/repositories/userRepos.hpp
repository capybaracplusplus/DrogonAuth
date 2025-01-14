#pragma once

#include "../models/user.hpp"
#include <drogon/HttpAppFramework.h>
#include "string"

class UserRepos {
    using Id = size_t;
    using DbClientType = drogon::orm::DbClientPtr;
    struct UserAuth {
        Id id_;
        std::string password;
    };
public:
    UserRepos() : dbClient_(drogon::app().getDbClient("default")) {}

public:
    void create_user(const user &);

    void deleteUser(const user &);

    user readUser(Id);

    void updatePassword(Id id, const std::string &password);

    UserAuth getUserAuthData(const std::string &username, const std::string &email);

private:
    DbClientType dbClient_;
};
