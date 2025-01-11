#pragma once

#include <drogon/HttpController.h>

using namespace drogon;

class AuthController : public drogon::HttpController<AuthController> {
public:

    METHOD_LIST_BEGIN

        ADD_METHOD_TO(AuthController::signUp, "/sign-up", Post, "AuthValidationMiddleware");
        ADD_METHOD_TO(AuthController::signIn, "/sign-in", Post, "AuthValidationMiddleware");
    METHOD_LIST_END

    //AuthMiddleware
    void signUp(const HttpRequestPtr &req,
                std::function<void(const HttpResponsePtr &)> &&callback);

    void signIn(const HttpRequestPtr &req,
                std::function<void(const HttpResponsePtr &)> &&callback);
};