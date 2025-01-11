#pragma once

#include <jwt-cpp/jwt.h>
#include <string>
#include <chrono>

class JwtToken {
public:
    struct TokenPair {
        std::string accessToken;
        std::string refreshToken;
    };

    JwtToken(const std::string &secretKey,
             size_t accessTokenLifetimeMinutes,
             size_t refreshTokenLifetimeDays)
            : secretKey_(secretKey),
              accessTokenLifetime_(std::chrono::minutes(accessTokenLifetimeMinutes)),
              refreshTokenLifetime_(std::chrono::days(refreshTokenLifetimeDays)) {}

    TokenPair createPair(const size_t &userId) {
        return createPair(std::to_string(userId));
    }

    TokenPair createPair(const std::string &userId) {
        auto now = std::chrono::system_clock::now();

        std::string accessToken = jwt::create()
                .set_issuer("Capy")
                .set_subject(userId)
                .set_issued_at(now)
                .set_expires_at(now + accessTokenLifetime_)
                .sign(jwt::algorithm::hs256{secretKey_});

        std::string refreshToken = jwt::create()
                .set_issuer("Capy")
                .set_subject(userId)
                .set_issued_at(now)
                .set_expires_at(now + refreshTokenLifetime_)
                .sign(jwt::algorithm::hs256{secretKey_});

        return {accessToken, refreshToken};
    }

private:
    std::string secretKey_;
    std::chrono::system_clock::duration accessTokenLifetime_;
    std::chrono::system_clock::duration refreshTokenLifetime_;
};