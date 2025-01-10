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

private:
    drogon::orm::DbClientPtr dbClient_;
};
