#include "userRepos.hpp"

void UserRepos::create_user(const user &user) {
    std::promise<void> promise;
    std::future<void> future = promise.get_future();
    dbClient_->execSqlAsync(
            "INSERT INTO users (username, hashpassword, email) VALUES ($1, $2, $3)",
            [=, &promise](const drogon::orm::Result &result) {
                LOG_INFO << "user created successfully: " << user.getUsername();
                promise.set_value();
            },
            [=, &promise](const std::exception_ptr &e) {
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

UserRepos::UserAuth UserRepos::getUserAuthData(const std::string &username, const std::string &email) try {
    auto users = dbClient_->execSqlSync(
            "SELECT Id, hashpassword FROM users WHERE username = $1 AND email = $2",
            username, email
    );
    if (!users.empty()) {
        auto user = users.front();
        size_t userId = user["Id"].as<int>();
        std::string passwordHash = user["hashpassword"].as<std::string>();
        return {userId, passwordHash};
    }
    return {0, ""};
} catch (const std::exception &e) {
    LOG_ERROR << "Error while querying database: " << e.what();
    return {0, ""};
}

void UserRepos::updatePassword(Id id, const std::string &password) {
    std::promise<void> promise;
    std::future<void> future = promise.get_future();

    dbClient_->execSqlAsync(
            "UPDATE users SET hashpassword = $1 WHERE Id = $2",
            [=, &promise](const drogon::orm::Result &result) {
                LOG_INFO << "Password updated successfully for user ID: " << id;
                promise.set_value();
            },
            [=, &promise](const std::exception_ptr &e) {
                try {
                    std::rethrow_exception(e);
                } catch (const std::exception &ex) {
                    LOG_ERROR << "Error updating password: " << ex.what();
                    promise.set_exception(std::current_exception());
                }
            },
            password, std::to_string(id)
    );

    future.get();
}

void UserRepos::deleteUser(const user &) {}

user UserRepos::readUser(Id) {}