#pragma once

#include <drogon/HttpController.h>

using namespace drogon;

class AuthController : public drogon::HttpController<AuthController> {
public:

    METHOD_LIST_BEGIN

        ADD_METHOD_TO(AuthController::signUp, "/sign-up", Post, "AuthMiddleware");
        ADD_METHOD_TO(AuthController::signIn, "/sign-in", Post, "AuthMiddleware");
    METHOD_LIST_END


    void signUp(const HttpRequestPtr &req,
                std::function<void(const HttpResponsePtr &)> &&callback);

    void signIn(const HttpRequestPtr &req,
                std::function<void(const HttpResponsePtr &)> &&callback);
};