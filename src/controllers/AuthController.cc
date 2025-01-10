#include "AuthController.h"
#include "../models/User.hpp"
#include "../../libs/Bcrypt.cpp/include/bcrypt.h"
#include "../services/serviceAuth.hpp"

void AuthController::signUp(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    std::clog << "log signUpController" << std::endl;
    try {
        auto body = req->getJsonObject();

        User user((*body)["username"].asString(), bcrypt::generateHash((*body)["password"].asString()),
                  (*body)["email"].asString());
        AuthService authService;
        authService.registration(user);
        Json::Value ret;
        ret["message"] = "success";
        auto resp = HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(drogon::HttpStatusCode::k200OK);
        callback(resp);

    } catch (const std::exception &e) {
        auto resp = drogon::HttpResponse::newHttpJsonResponse({{"error", e.what()}});
        resp->setStatusCode(drogon::k400BadRequest);
        callback(resp);
    }
}

void AuthController::signIn(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    std::clog << "log signIpController" << std::endl;
    try {
        auto body = req->getJsonObject();
        User user((*body)["username"].asString(), (*body)["password"].asString(),
                  (*body)["email"].asString());
        AuthService authService;
        authService.login(user);

    } catch (const std::exception &e) {
        auto resp = drogon::HttpResponse::newHttpJsonResponse({{"error", e.what()}});
        resp->setStatusCode(drogon::k400BadRequest);
        callback(resp);
    }
}
