#pragma once

#include "../models/User.hpp"
#include "../utils/jwt/jwtToken.hpp"

class AuthService {
public:
    using id = size_t;
    struct UserData {
        JwtToken::TokenPair TokenPair;
        id id;
    };
public:
    void registration(const User &);

    UserData login(const User &);

};