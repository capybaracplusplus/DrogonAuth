#pragma once

#include <drogon/HttpMiddleware.h>
#include <string>

#include "../dto/userDtro.hpp"

using namespace drogon;

void validationFunc(const UserDto &) noexcept(false) {}

class AuthMiddleware  : public HttpMiddleware<AuthMiddleware > {
public:

    AuthMiddleware () {};

    void invoke(const HttpRequestPtr &req,
                MiddlewareNextCallback &&nextCb,
                MiddlewareCallback &&mcb) override {

        std::clog << "log AuthMiddleware" << std::endl;


        try {

        } catch (const std::exception &e) {
            auto resp = drogon::HttpResponse::newHttpJsonResponse({{"error", e.what()}});
            resp->setStatusCode(drogon::k400BadRequest);
            mcb(resp);
            return;
        }
        nextCb(std::move(mcb));
    }
};