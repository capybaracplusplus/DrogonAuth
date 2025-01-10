#include <drogon/HttpMiddleware.h>
#include <string>

#include "../dto/userDtro.hpp"

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

        if (!body->isMember("username") || !body->isMember("password") || !body->isMember("email")) {
            auto resp = drogon::HttpResponse::newHttpJsonResponse(
                    {{"error", "Required fields are missing in the body"}});
            resp->setStatusCode(drogon::k400BadRequest);
            mcb(resp);
            return;
        }

        UserDto user((*body)["username"].asString(), (*body)["password"].asString(),
                     (*body)["email"].asString());
        try {
            validationFunc(user);

        } catch (const std::exception &e) {
            auto resp = drogon::HttpResponse::newHttpJsonResponse({{"error", e.what()}});
            resp->setStatusCode(drogon::k400BadRequest);
            mcb(resp);
            return;
        }
        nextCb(std::move(mcb));
    }
};