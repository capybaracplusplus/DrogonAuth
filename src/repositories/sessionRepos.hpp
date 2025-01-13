#pragma once

#include <sw/redis++/redis++.h>
#include "../utils/jwt/jwtToken.hpp"

//using namespace sw::redis;

namespace repos {
    class Session {
    public:
        using DbClientType = sw::redis::Redis;
        using JwtTokens = JwtToken::TokenPair;
        using user_id = size_t;
    public:

        Session() = delete;

        Session(const JwtTokens &jwtTokens_) : jwtTokens_(jwtTokens_),
                                               dbClient_(sw::redis::Redis("tcp://127.0.0.1:6379")) {}

        void upload(const user_id &);

        const JwtTokens &get(const user_id &);

        void remove(user_id);

        const JwtTokens &getJwt();

    private:
        DbClientType dbClient_;
        JwtTokens jwtTokens_;
    };
}



