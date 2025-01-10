#include "serviceAuth.hpp"
#include "../repositories/UserTableRepos.hpp"
#include "../../libs/Bcrypt.cpp/include/bcrypt.h"
#include "jwt-cpp/jwt.h"

void AuthService::registration(const User &user) {
    std::clog << "log serviceRegistraion" << std::endl;
    try {
        UserRepos userRepos;
        userRepos.create_user(user);
    } catch (...) {
        std::clog << "err serviceRegistraion" << std::endl;
        throw;
    }
}

void AuthService::login(const User &user) {
    UserRepos repos;
    if (!bcrypt::validatePassword(user.getHashPassword_(),
                                  repos.find_user_hashPassword(user.getUsername(), user.getEmail()))) {
        throw std::runtime_error("Incorrect password");
    }

    const auto accToken = jwt::create()
            .set_issued_now()
            .set_expires_in(std::chrono::seconds{3600})
            .sign(jwt::algorithm::hs256{"secret"});

    const auto refToken = jwt::create()
            .set_issued_now()
            .set_expires_in(std::chrono::seconds{6480000})
            .sign(jwt::algorithm::hs256{"secret"});


}