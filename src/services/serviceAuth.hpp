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
    static void registration(const User &);

    static UserData login(const User &);

    static void logout(const UserData &);
};