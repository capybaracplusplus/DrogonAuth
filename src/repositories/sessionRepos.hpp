#pragma once

#include <sw/redis++/redis++.h>
#include "../utils/jwt/jwtToken.hpp"

//using namespace sw::redis;

namespace repos {
    extern sw::redis::Redis redisDbClient;

    class Session {
    public:
        using DbClientType = sw::redis::Redis;
        using JwtTokens = JwtToken::TokenPair;
        using user_id = size_t;
    public:

        Session() = delete;

        Session(const JwtTokens &jwtTokens_) : jwtTokens_(jwtTokens_) {}

        void upload(const user_id &) ;

        const JwtTokens &get(const user_id &) ;

        void remove(const user_id &) ;

        const JwtTokens &getJwt() ;

    private:
        DbClientType &dbClient_ = redisDbClient;
        JwtTokens jwtTokens_;
    };
}




