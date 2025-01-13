#pragma once

#include <drogon/HttpMiddleware.h>
#include <string>
#include "../utils/jwt/jwtToken.hpp"
#include "../dto/userDtro.hpp"
#include "../repositories/sessionRepos.hpp"

using namespace drogon;

size_t getUserId(const std::string &refreshToken) {
    auto decodedToken = jwt::decode(refreshToken);
    return std::stoi(decodedToken.get_payload_claim("sub").as_string());
}

class AuthMiddleware : public HttpMiddleware<AuthMiddleware> {
public:
    AuthMiddleware() {};

    void invoke(const HttpRequestPtr &req,
                MiddlewareNextCallback &&nextCb,
                MiddlewareCallback &&mcb) override {

        std::clog << "log AuthMiddleware" << std::endl;

        auto body = req->getJsonObject();
        if (!body) {
            Json::Value ret;
            ret["error"] = "Invalid JSON body";
            auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
            resp->setStatusCode(drogon::k400BadRequest);
            mcb(resp);
            return;
        }

        if (!body->isMember("accessToken")) {
            Json::Value ret;
            ret["error"] = "The body is missing the required field accessToken";
            auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
            resp->setStatusCode(drogon::k400BadRequest);
            mcb(resp);
            return;
        }

        auto cookies = req->getCookies();

        auto refreshTokenIt = cookies.find("refreshToken");

        if (refreshTokenIt == cookies.end()) {
            Json::Value ret;
            ret["error"] = "Missing refreshToken in cookies";
            auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
            resp->setStatusCode(drogon::k401Unauthorized);
            mcb(resp);
            return;
        }

        static JwtToken jwtValidateToken = JwtToken();

        std::string refreshToken = refreshTokenIt->second;
        std::string accessToken = (*body)["accessToken"].asString();

        if (!jwtValidateToken.validateToken(accessToken)) {
            if (!jwtValidateToken.validateToken(refreshToken)) {
                Json::Value ret;
                ret["error"] = "Refresh token expired";
                auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
                resp->setStatusCode(drogon::k401Unauthorized);
                mcb(resp);
                return;
            }
            try {
                auto decodedToken = jwt::decode(refreshToken);
                auto userId = std::stoi(decodedToken.get_payload_claim("sub").as_string());
                auto redisTokenPair = repos::Session(repos::Session::JwtTokens{accessToken, refreshToken}).get(userId);
                if (redisTokenPair.refreshToken != refreshToken) {
                    Json::Value ret;
                    ret["error"] = "Refresh token is not valid";
                    auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
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
            } catch (const std::exception &e) {
                Json::Value ret;
                ret["error"] = "Invalid refreshToken format";
                auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
                resp->setStatusCode(drogon::k400BadRequest);
                mcb(resp);
                return;
            }
        } else {
            try {
                auto decodedToken = jwt::decode(accessToken);
                auto userId = std::stoi(decodedToken.get_payload_claim("sub").as_string());


                auto redisTokenPair = repos::Session(repos::Session::JwtTokens{accessToken, refreshToken}).get(userId);


                if (redisTokenPair.accessToken != accessToken) {
                    Json::Value ret;
                    ret["error"] = "accessToken token is not valid";
                    auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
                    resp->setStatusCode(drogon::k401Unauthorized);
                    mcb(resp);
                    return;
                }
                repos::Session s(JwtToken::TokenPair{accessToken, refreshToken});
                s.remove(userId);
                s.upload(userId);
                req->getAttributes()->insert("newAccessToken", accessToken);
                req->getAttributes()->insert("newRefreshToken", refreshToken);
            } catch (const std::exception &e) {
                Json::Value ret;
                ret["error"] = "Invalid refreshToken format";
                auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
                resp->setStatusCode(drogon::k400BadRequest);
                mcb(resp);
                return;
            }
        }
        // в контроллере
        //auto attributes = req->getAttributes();
        //auto newAccessToken = attributes->get<std::string>("newAccessToken");
        //auto newRefreshToken = attributes->get<std::string>("newRefreshToken");

        nextCb(std::move(mcb));
    }
};