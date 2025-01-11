#pragma once

#include "../models/User.hpp"
#include "../utils/jwt/jwtToken.hpp"

class AuthService {
public:
    void registration(const User &);

    JwtToken::TokenPair login(const User &);

};