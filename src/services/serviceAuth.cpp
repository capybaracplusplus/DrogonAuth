#include "serviceAuth.hpp"
#include "../repositories/UserTableRepos.hpp"
#include "../../libs/Bcrypt.cpp/include/bcrypt.h"
#include "jwt-cpp/jwt.h"
#include "../repositories/sessionRepos.hpp"

void AuthService::registration(const User &user) {
    std::clog << "log registration" << std::endl;
    try {
        UserRepos userRepos;
        userRepos.create_user(user);
    } catch (const std::exception &ex) {
        std::clog << "err registration: " << ex.what() << std::endl;
        throw std::runtime_error("Error during user registration");
    }
}

AuthService::UserData AuthService::login(const User &user) {
    std::clog << "log login" << std::endl;
    try {
        UserRepos repos;
        auto userData = repos.getUserAuthData(user.getUsername(), user.getEmail());
        if (!bcrypt::validatePassword(user.getHashPassword_(),
                                      userData.password)) {
            throw std::runtime_error("Incorrect password");
        }
        JwtToken jwtToken("secretKey", 1800, 30);
        auto jwtTokenPair = jwtToken.createPair(userData.id_);
        repos::Session session(jwtTokenPair);
        session.upload(userData.id_);
        return {jwtTokenPair, userData.id_};
    } catch (...) {
        std::clog << "err login" << std::endl;
        throw;
    }
}