#pragma once

#include "../models/user.hpp"
#include "../utils/jwt/jwtToken.hpp"

class AuthService {
public:
    using Id = size_t;
    struct UserData {
        JwtToken::TokenPair TokenPair;
        Id id;
    };
public:
    static void registration(const user &);

    static UserData login(const user &);

    static void logout(const UserData &);
};