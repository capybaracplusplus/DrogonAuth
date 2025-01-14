#pragma once

#include <drogon/HttpMiddleware.h>
#include <string>
#include "../utils/jwt/jwtToken.hpp"
#include "../dto/userDtro.hpp"
#include "../repositories/sessionRepos.hpp"


class ValidateTokensMiddleware : public drogon::HttpMiddleware<ValidateTokensMiddleware> {
public:
    ValidateTokensMiddleware() {};

    void invoke(const drogon::HttpRequestPtr &req,
                drogon::MiddlewareNextCallback &&nextCb,
                drogon::MiddlewareCallback &&mcb) override {
        std::clog << "log ValidateTokensMiddleware" << std::endl;

        static JwtToken jwtValidateToken = JwtToken();
        auto attributes = req->getAttributes();
        std::string accessToken = attributes->get<std::string>("accessToken");
        std::string refreshToken = attributes->get<std::string>("refreshToken");

        try {
            if (!jwtValidateToken.validateToken(accessToken)) {
                if (!jwtValidateToken.validateToken(refreshToken)) {
                    std::runtime_error("Refresh token is not valid");
                }
                std::runtime_error("Access token is not valid");
            } else {
                auto decodedToken = jwt::decode(accessToken);
                auto userId = std::stoi(decodedToken.get_payload_claim("sub").as_string());

                JwtToken::TokenPair redisTokenPair = repos::Session(
                        repos::Session::JwtTokens{accessToken, refreshToken}).get(userId,
                                                                                  repos::Session::JwtTokens{accessToken,
                                                                                                            refreshToken});

                if (redisTokenPair.accessToken != accessToken) {
                    std::runtime_error("No such access token found");
                }
            }
        } catch (const std::exception &e) {
            Json::Value ret;
            ret["error"] = e.what();
            auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
            resp->setStatusCode(drogon::k401Unauthorized);
            mcb(resp);
            return;
        }
        nextCb(std::move(mcb));
    }
};