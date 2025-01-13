#pragma once

#include <drogon/HttpController.h>

using namespace drogon;

class authController : public drogon::HttpController<authController> {
public:

    METHOD_LIST_BEGIN

        ADD_METHOD_TO(authController::signUp, "/sign-up", Post, "authValidationMiddleware");
        ADD_METHOD_TO(authController::signIn, "/sign-in", Post, "authValidationMiddleware");
        ADD_METHOD_TO(authController::logout, "/logout", Post, "authMiddleware");

    METHOD_LIST_END

    void signUp(const HttpRequestPtr &req,
                std::function<void(const HttpResponsePtr &)> &&callback);

    void signIn(const HttpRequestPtr &req,
                std::function<void(const HttpResponsePtr &)> &&callback);

    void logout(const HttpRequestPtr &req,
                std::function<void(const HttpResponsePtr &)> &&callback);

    void changePassword(const HttpRequestPtr &req,
                        std::function<void(const HttpResponsePtr &)> &&callback);
};