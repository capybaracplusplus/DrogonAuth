#include "UserTableRepos.hpp"

void UserRepos::create_user(const User &user) {
    dbClient_->execSqlAsync(
            "INSERT INTO users (username, hashpassword, email) VALUES ($1, $2, $3)",
            [=](const drogon::orm::Result &result) {
                LOG_INFO << "User created successfully: " << user.getUsername();
            },
            [=](const std::exception_ptr &e) {
                try {
                    std::rethrow_exception(e);
                } catch (const std::exception &ex) {
                    LOG_ERROR << "Error creating user: " << ex.what();
                }
            },
            user.getUsername(), user.getHashPassword_(), user.getEmail()
    );
}

std::string UserRepos::find_user_hashPassword(const std::string &username, const std::string &email) {
    try {
        auto users = dbClient_->execSqlSync(
                "SELECT hashpassword FROM users WHERE username = $1 AND email = $2",
                username, email
        );
        if (!users.empty()) {
            return users.front()["hashpassword"].as<std::string>();
        }
        return "";
    } catch (const std::exception &e) {
        LOG_ERROR << "Error while querying database: " << e.what();
        return "";
    }
}

void UserRepos::delete_user(const User &) {}

User UserRepos::read_user(id) {}

void UserRepos::update_user(const User &) {}