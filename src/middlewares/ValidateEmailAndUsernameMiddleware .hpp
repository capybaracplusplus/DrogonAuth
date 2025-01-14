#pragma once

#include <drogon/HttpMiddleware.h>
#include <json/json.h>
#include <regex>

using namespace drogon;

class ValidateEmailAndUsernameMiddleware : public HttpMiddleware<ValidateEmailAndUsernameMiddleware> {
public:
    ValidateEmailAndUsernameMiddleware() {};

    void invoke(const HttpRequestPtr &req,
                MiddlewareNextCallback &&nextCb,
                MiddlewareCallback &&mcb) override {
        auto attributes = req->getAttributes();
        auto body = attributes->get<std::shared_ptr<Json::Value>>("body");

        try {
            if (!body->isMember("username") || !body->isMember("email")) {
                throw std::runtime_error("Required fields (username or email) are missing");
            }
            std::string email = (*body)["email"].asString();
            std::regex emailRegex("^[a-zA-Z0-9_+&*-]+(?:\\.[a-zA-Z0-9_+&*-]+)*@(?:[a-zA-Z0-9-]+\\.)+[a-zA-Z]{2,7}$");

            if (!std::regex_match(email, emailRegex)) {
                throw std::runtime_error("Invalid email format");
            }
            std::string username = (*body)["username"].asString();

            if (username.empty()) {
                throw std::runtime_error("Username cannot be empty");
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
