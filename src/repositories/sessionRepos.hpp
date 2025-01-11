#pragma once

#include <sw/redis++/redis++.h>
#include "../utils/jwt/jwtToken.hpp"

class Session {
    using DbClientType = int;
    using JwtTokens = JwtToken::TokenPair;
    using user_id = size_t;
public:

    Session() = delete;

    Session(const JwtTokens& jwtTokens_) : jwtTokens_(jwtTokens_) {}

    void upload();

    const Session &get(user_id);

    void remove(user_id);

private:
    DbClientType dbClient_;
    JwtTokens jwtTokens_;
};


