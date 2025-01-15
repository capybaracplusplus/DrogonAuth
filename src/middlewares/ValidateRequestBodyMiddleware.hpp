#pragma once

#include <drogon/HttpMiddleware.h>
#include <string>

#include "../dto/userDtro.hpp"

using namespace drogon;

void validationFunc(const UserDto &) noexcept(false) {}

class ValidateRequestBodyMiddleware : public HttpMiddleware<ValidateRequestBodyMiddleware> {
public:

    ValidateRequestBodyMiddleware() {};

    void invoke(const HttpRequestPtr &req,
                MiddlewareNextCallback &&nextCb,
                MiddlewareCallback &&mcb) override {
        std::clog << "log ValidateRequestBodyMiddleware" << std::endl;

        try {
            if (req->getBody().empty()) {
                std::clog << "Request body is empty" << std::endl;
                throw std::runtime_error("Empty request body");
            }
            auto body = req->getJsonObject();

            if (!body) {
                std::clog << "Failed to parse JSON body" << std::endl;
                throw std::runtime_error("Invalid JSON body");
            }
            req->getAttributes()->insert("body", body);
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