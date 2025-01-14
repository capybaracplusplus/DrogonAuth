#include "serviceAuth.hpp"
#include "../repositories/userRepos.hpp"
#include "../../libs/Bcrypt.cpp/include/bcrypt.h"
#include "jwt-cpp/jwt.h"
#include "../repositories/sessionRepos.hpp"

void AuthService::registration(const user &user) {
    std::clog << "log AuthService::registration" << std::endl;
    try {
        static UserRepos repos;
        repos.create_user(user);
    } catch (const std::exception &ex) {
        std::clog << "err registration: " << ex.what() << std::endl;
        throw std::runtime_error("Error during user registration");
    }
}

AuthService::UserData AuthService::login(const user &user) {
    std::clog << "log AuthService::login" << std::endl;
    try {
        static UserRepos repos;
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

void AuthService::logout(const UserData &userData) {
    std::clog << "log AuthService::logout" << std::endl;
    repos::Session session(userData.TokenPair);
    session.remove(userData.id, userData.TokenPair);
}

JwtToken::TokenPair AuthService::updateAccessToken(const UserData &userData) {
    std::clog << "log AuthService::updateAccessToken" << std::endl;
    repos::Session session(userData.TokenPair);
    session.remove(userData.id, userData.TokenPair);
    session.upload(userData.id);
    return session.get(userData.id, userData.TokenPair);
}


void AuthService::changePassword(Id id, const std::string &password) {
    std::clog << "log AuthService::changePassword" << std::endl;
    static UserRepos repos;
    repos.updatePassword(id, password);
}