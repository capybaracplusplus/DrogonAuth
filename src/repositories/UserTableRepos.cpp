#include "UserTableRepos.hpp"

void UserRepos::create_user(const User &user) {
    std::promise<void> promise;
    std::future<void> future = promise.get_future();
    dbClient_->execSqlAsync(
            "INSERT INTO users (username, hashpassword, email) VALUES ($1, $2, $3)",
            [=,&promise](const drogon::orm::Result &result) {
                LOG_INFO << "User created successfully: " << user.getUsername();
                promise.set_value();
            },
            [=,&promise](const std::exception_ptr &e) {
                try {
                    std::rethrow_exception(e);
                } catch (const std::exception &ex) {
                    LOG_ERROR << "Error creating user: " << ex.what();
                    promise.set_exception(std::current_exception());
                }
            },
            user.getUsername(), user.getHashPassword_(), user.getEmail()
    );
    future.get();
}

UserRepos::UserAuth UserRepos::getUserAuthData(const std::string &username, const std::string &email) {
    try {
        auto users = dbClient_->execSqlSync(
                "SELECT id, hashpassword FROM users WHERE username = $1 AND email = $2",
                username, email
        );
        if (!users.empty()) {
            auto user = users.front();
            size_t userId = user["id"].as<int>();
            std::string passwordHash = user["hashpassword"].as<std::string>();
            return {userId, passwordHash};
        }
        return {0, ""};
    } catch (const std::exception &e) {
        LOG_ERROR << "Error while querying database: " << e.what();
        return {0, ""};
    }
}

void UserRepos::delete_user(const User &) {}

User UserRepos::read_user(id) {}

void UserRepos::update_user(const User &) {}