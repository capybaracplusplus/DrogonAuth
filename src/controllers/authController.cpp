#include "authController.h"
#include "../models/user.hpp"
#include "../../libs/Bcrypt.cpp/include/bcrypt.h"
#include "../services/serviceAuth.hpp"
#include "../repositories/sessionRepos.hpp"

void authController::signUp(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    std::clog << "log authController::signUp" << std::endl;
    try {
        auto body = req->getJsonObject();
        user user((*body)["username"].asString(), bcrypt::generateHash((*body)["password"].asString()),
                  (*body)["email"].asString());

        AuthService::registration(user);

        Json::Value ret;
        ret["message"] = "user registered successfully";
        auto resp = HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(drogon::HttpStatusCode::k201Created);
        callback(resp);
    } catch (const std::exception &e) {
        Json::Value ret;
        ret["error"] = e.what();
        auto resp = drogon::HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(drogon::k400BadRequest);
        callback(resp);
    }
}

void authController::signIn(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    std::clog << "log authController::signIn" << std::endl;
    try {
        auto body = req->getJsonObject();
        user user((*body)["username"].asString(), (*body)["password"].asString(),
                  (*body)["email"].asString());

        auto userData = AuthService::login(user);

        auto jwt = userData.TokenPair;

        Json::Value ret;
        ret["message"] = "The user has successfully logged into the account";
        ret["accessToken"] = jwt.accessToken;
        ret["userId"] = userData.id;
        auto resp = HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(drogon::HttpStatusCode::k204NoContent);
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

void authController::logout(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    std::clog << "log authController::logoutController" << std::endl;
    try {
        auto attributes = req->getAttributes();
        auto accessToken = attributes->get<std::string>("accessToken");
        auto refreshToken = attributes->get<std::string>("refreshToken");

        auto decodedToken = jwt::decode(refreshToken);
        auto userId = std::stoi(decodedToken.get_payload_claim("sub").as_string());

        AuthService::UserData userData(JwtToken::TokenPair{accessToken, refreshToken}, userId);

        AuthService::logout(userData);

        Json::Value ret;
        ret["message"] = "The user has successfully logged out of the account.";
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

void authController::getNewAccessToken(const HttpRequestPtr &req,
                                       std::function<void(const HttpResponsePtr &)> &&callback) {
    std::clog << "log authController::getNewAccessToken" << std::endl;

    try {
        auto attributes = req->getAttributes();
        auto accessToken = attributes->get<std::string>("accessToken");
        auto refreshToken = attributes->get<std::string>("refreshToken");

        auto decodedToken = jwt::decode(refreshToken);
        auto userId = std::stoi(decodedToken.get_payload_claim("sub").as_string());

        AuthService::UserData userData(JwtToken::TokenPair{accessToken, refreshToken}, userId);
        JwtToken::TokenPair jwt = AuthService::updateAccessToken(userData);

        Json::Value ret;
        ret["message"] = "The user has successfully updated the access token";
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

void
authController::changePassword(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    std::clog << "log authController::changePassword" << std::endl;

    try {
        auto attributes = req->getAttributes();
        auto body = attributes->get<std::shared_ptr<Json::Value>>("body");
        std::string hashPassword = bcrypt::generateHash((*body)["password"].asString());
        std::string accessToken = (*body)["accessToken"].asString();
        auto decodedToken = jwt::decode(accessToken);
        AuthService::Id userId = std::stoi(decodedToken.get_payload_claim("sub").as_string());
        AuthService::changePassword(userId, hashPassword);
        Json::Value ret;
        ret["message"] = "New password set successfully";
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