#pragma once

#include "../models/User.hpp"
#include "../utils/jwt/jwtToken.hpp"

class AuthService {
public:
    using Id = size_t;
    struct UserData {
        JwtToken::TokenPair TokenPair;
        Id id;
    };
public:
    void registration(const User &);

    UserData login(const User &);

    void logout(const UserData &);
};