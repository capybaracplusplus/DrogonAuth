#pragma once
#include <drogon/HttpController.h>
using namespace drogon;

class AuthController : public drogon::HttpController<AuthController> {
public:

    METHOD_LIST_BEGIN

        METHOD_ADD(AuthController::signUp, "/sign-up", Post, "AuthMiddleware");
        METHOD_ADD(AuthController::signIn, "/sign-in", Post);


    METHOD_LIST_END


    void signUp(const HttpRequestPtr &req,
                std::function<void(const HttpResponsePtr &)> &&callback);
    void signIn(const HttpRequestPtr &req,
                std::function<void(const HttpResponsePtr &)> &&callback);
};
