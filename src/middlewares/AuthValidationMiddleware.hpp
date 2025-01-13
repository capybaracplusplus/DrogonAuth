#include <drogon/HttpMiddleware.h>
#include <string>

#include "../dto/userDtro.hpp"

using namespace drogon;
void validationFunc(const UserDto &) noexcept(false) {}

class AuthValidationMiddleware : public HttpMiddleware<AuthValidationMiddleware> {
public:

    AuthValidationMiddleware() {};

    void invoke(const HttpRequestPtr &req,
                MiddlewareNextCallback &&nextCb,
                MiddlewareCallback &&mcb) override {

        std::clog << "log AuthValidationMiddleware" << std::endl;

        if (req->getBody().empty()) {
            std::clog << "Request body is empty" << std::endl;
            Json::Value ret;
            ret["error"] = "Empty request body";
            auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
            resp->setStatusCode(drogon::k400BadRequest);
            mcb(resp);
            return;
        }

        auto body = req->getJsonObject();

        if (!body) {
            std::clog << "Failed to parse JSON body" << std::endl;
            Json::Value ret;
            ret["error"] = "Invalid JSON body";
            auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
            resp->setStatusCode(drogon::k400BadRequest);
            mcb(resp);
            return;
        }

        if (!body->isMember("username") || !body->isMember("password") || !body->isMember("email")) {
            std::clog << "Missing required fields: username, password, or email" << std::endl;
            Json::Value ret;
            ret["error"] = "Required fields are missing in the body";
            auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
            resp->setStatusCode(drogon::k400BadRequest);
            mcb(resp);
            return;
        }

        UserDto user((*body)["username"].asString(), (*body)["password"].asString(),
                     (*body)["email"].asString());

        if (user.getUsername().empty() || user.getPassword_().empty() || user.getEmail().empty()) {
            std::clog << "One of the required fields is empty" << std::endl;
            Json::Value ret;
            ret["error"] = "One or more required fields are empty";
            auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
            resp->setStatusCode(drogon::k400BadRequest);
            mcb(resp);
            return;
        }
        try {
            validationFunc(user);

        } catch (const std::exception &e) {
            Json::Value ret;
            ret["error"] =  e.what();
            auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
            resp->setStatusCode(drogon::k400BadRequest);
            mcb(resp);
            return;
        }
        nextCb(std::move(mcb));
    }
};