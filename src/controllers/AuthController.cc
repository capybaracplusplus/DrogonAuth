#include "AuthController.h"
#include "../services/serviceRegistraion.hpp"
#include "../models/User.hpp"

void AuthController::signUp(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    std::clog << "log signUpController" << std::endl;

    try {
        auto body = req->getJsonObject();

        User user((*body)["username"].asString(), (*body)["hashPassword"].asString(), (*body)["email"].asString());

        serviceRegistraion(user);

        Json::Value ret;
        ret["message"] = "success";
        auto resp = HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(drogon::HttpStatusCode::k200OK);
        callback(resp);

    } catch (...) {
        std::clog << "err AuthController::signUp" << std::endl;
        throw;
    }
}

void AuthController::signIn(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    // write your application logic here
}
