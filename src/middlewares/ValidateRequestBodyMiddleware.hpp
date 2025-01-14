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

class ValidateRequestBodyMiddleware : public HttpMiddleware<ValidateRequestBodyMiddleware> {
public:
    ValidateRequestBodyMiddleware() {};

    void invoke(const HttpRequestPtr &req,
                MiddlewareNextCallback &&nextCb,
                MiddlewareCallback &&mcb) override {
        std::clog << "log ValidateRequestBodyMiddleware" << std::endl;

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

        std::string refreshToken = refreshTokenIt->second;
        std::string accessToken = (*body)["accessToken"].asString();

        req->getAttributes()->insert("accessToken", accessToken);
        req->getAttributes()->insert("refreshToken", refreshToken);

        nextCb(std::move(mcb));
    }
};