#pragma once

#include <drogon/HttpMiddleware.h>
#include <string>
#include "../utils/jwt/jwtToken.hpp"
#include "../dto/userDtro.hpp"
#include "../repositories/sessionRepos.hpp"

using namespace drogon;

void validationFunc(const UserDto &) noexcept(false) {}

class AuthMiddleware : public HttpMiddleware<AuthMiddleware> {
public:
    AuthMiddleware() {};

    void invoke(const HttpRequestPtr &req,
                MiddlewareNextCallback &&nextCb,
                MiddlewareCallback &&mcb) override {

        std::clog << "log AuthMiddleware" << std::endl;

        auto body = req->getJsonObject();

        if (!body) {
            auto resp = drogon::HttpResponse::newHttpJsonResponse(
                    {{"error", "Invalid JSON body"}});
            resp->setStatusCode(drogon::k400BadRequest);
            mcb(resp);
            return;
        }

        if (!body->isMember("accessToken")) {
            auto resp = drogon::HttpResponse::newHttpJsonResponse(
                    {{"error", "The body is missing the required field accessToken"}});
            resp->setStatusCode(drogon::k400BadRequest);
            mcb(resp);
            return;
        } else if (!body->isMember("userId")) {
            auto resp = drogon::HttpResponse::newHttpJsonResponse(
                    {{"error", "The body is missing the required field userId"}});
            resp->setStatusCode(drogon::k400BadRequest);
            mcb(resp);
            return;
        }

        auto cookies = req->getCookies();

        auto refreshTokenIt = cookies.find("refreshToken");

        if (refreshTokenIt == cookies.end()) {
            auto resp = drogon::HttpResponse::newHttpJsonResponse(
                    {{"error", "Missing refreshToken in cookies"}});
            resp->setStatusCode(drogon::k401Unauthorized);
            mcb(resp);
            return;
        }

        static JwtToken jwtValidateToken = JwtToken();

        std::string refreshToken = refreshTokenIt->second;
        std::string accessToken = (*body)["accessToken"].asString();
        repos::Session::user_id userId = std::stoi((*body)["userId"].asString());

        if (!jwtValidateToken.validateToken(accessToken)) {
            if (!jwtValidateToken.validateToken(refreshToken)) {
                auto resp = drogon::HttpResponse::newHttpJsonResponse(
                        {{"error", "Refresh token expired"}});
                resp->setStatusCode(drogon::k401Unauthorized);
                mcb(resp);
                return;
            }
            auto redisTokenPair = repos::Session(repos::Session::JwtTokens{accessToken, refreshToken}).get(userId);
            if (redisTokenPair.refreshToken != refreshToken) {
                auto resp = drogon::HttpResponse::newHttpJsonResponse(
                        {{"error", "Refresh token is not valid"}});
                resp->setStatusCode(drogon::k401Unauthorized);
                mcb(resp);
                return;
            }
            auto newJwtTokenPair = JwtToken().createPair(userId);
            repos::Session session(newJwtTokenPair);
            session.remove(userId);
            session.upload(userId);
            req->getAttributes()->insert("newAccessToken", newJwtTokenPair.accessToken);
            req->getAttributes()->insert("newRefreshToken", newJwtTokenPair.refreshToken);
            // в контроллере
            //auto attributes = req->getAttributes();
            //auto newAccessToken = attributes->getOptional<std::string>("newAccessToken");
            //auto newRefreshToken = attributes->getOptional<std::string>("newRefreshToken");
        }
        nextCb(std::move(mcb));
    }
};