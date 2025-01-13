#include "AuthController.h"
#include "../models/User.hpp"
#include "../../libs/Bcrypt.cpp/include/bcrypt.h"
#include "../services/serviceAuth.hpp"
#include "../repositories/sessionRepos.hpp"

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
        Json::Value ret;
        ret["error"] = e.what();
        auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
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
        static AuthService authService;
        auto userData = authService.login(user);
        auto jwt = userData.TokenPair;
        Json::Value ret;
        ret["message"] = "success";
        ret["accessToken"] = jwt.accessToken;
        ret["userId"] = userData.id;
        auto resp = HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(drogon::HttpStatusCode::k200OK);
        Cookie cookie("refreshToken", jwt.refreshToken);
        cookie.setPath("/");
        cookie.setHttpOnly(true);
        cookie.setSecure(true);
        resp->addCookie(cookie);
        callback(resp);
    } catch (const std::exception &e) {
        Json::Value ret;
        ret["error"] = e.what();
        auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(drogon::k400BadRequest);
        callback(resp);
    }
}

void AuthController::logout(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    std::clog << "log logoutController" << std::endl;
    try {
        auto attributes = req->getAttributes();
        auto newAccessToken = attributes->get<std::string>("newAccessToken");
        auto newRefreshToken = attributes->get<std::string>("newRefreshToken");

        auto decodedToken = jwt::decode(newAccessToken);
        auto userId = std::stoi(decodedToken.get_payload_claim("sub").as_string());

        AuthService::UserData userData(JwtToken::TokenPair{newAccessToken, newRefreshToken}, userId);

        static AuthService authService;
        authService.logout(userData);

        Json::Value ret;
        ret["message"] = "success";
        ret["logout"] = "ok";
        auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(drogon::k200OK);
        callback(resp);
    } catch (const std::exception &e) {
        Json::Value ret;
        ret["error"] = e.what();
        auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(drogon::k400BadRequest);
        callback(resp);
    }
}

void
AuthController::changePassword(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {

}