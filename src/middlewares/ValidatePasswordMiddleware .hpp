#pragma once

#include <drogon/HttpMiddleware.h>
#include <json/json.h>
#include <regex>

using namespace drogon;

class ValidatePasswordMiddleware : public HttpMiddleware<ValidatePasswordMiddleware> {
public:
    ValidatePasswordMiddleware() {};

    void invoke(const HttpRequestPtr &req,
                MiddlewareNextCallback &&nextCb,
                MiddlewareCallback &&mcb) override {
        auto attributes = req->getAttributes();
        auto body = attributes->get<std::shared_ptr<Json::Value>>("body");

        try {
            if (!body->isMember("password")) {
                throw std::runtime_error("Required field password is missing");
            }

            std::string password = (*body)["password"].asString();

            if (password.length() < 8) {
                throw std::runtime_error("Password must be at least 8 characters long");
            }
        } catch (const std::exception &e) {
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
