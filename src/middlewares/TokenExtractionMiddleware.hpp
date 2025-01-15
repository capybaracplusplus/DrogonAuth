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

class TokenExtractionMiddleware : public HttpMiddleware<TokenExtractionMiddleware> {
public:
    TokenExtractionMiddleware() {};

    void invoke(const HttpRequestPtr &req,
                MiddlewareNextCallback &&nextCb,
                MiddlewareCallback &&mcb) override {
        std::clog << "log TokenExtractionMiddleware" << std::endl;

        try {
            auto body = req->getJsonObject();
            if (!body) {
                throw std::runtime_error("Invalid JSON body");
            }

            if (!body->isMember("accessToken")) {
                throw std::runtime_error("The body is missing the required field accessToken");
            }

            auto cookies = req->getCookies();
            auto refreshTokenIt = cookies.find("refreshToken");

            if (refreshTokenIt == cookies.end()) {
                throw std::runtime_error("Missing refreshToken in cookies");
            }

            std::string refreshToken = refreshTokenIt->second;
            std::string accessToken = (*body)["accessToken"].asString();

            req->getAttributes()->insert("accessToken", accessToken);
            req->getAttributes()->insert("refreshToken", refreshToken);
        } catch (const std::exception &e) {
            std::clog << "log "<< e.what() << std::endl;
            Json::Value ret;
            ret["error"] = e.what();
            auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
            resp->setStatusCode(drogon::k400BadRequest);
            mcb(resp);
            return;
        }
        nextCb(std::move(mcb));
    }
};