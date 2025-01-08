#include <drogon/HttpMiddleware.h>
#include <string>

using namespace drogon;

class AuthMiddleware : public HttpMiddleware<AuthMiddleware> {
public:

    AuthMiddleware() {};// do not omit constructor

    void invoke(const HttpRequestPtr &req,
                MiddlewareNextCallback &&nextCb,
                MiddlewareCallback &&mcb) override {

        const std::string &origin = req->getHeader("Origin");
        if (origin.empty() || origin != "https://localhost:3000") {
            auto resp = drogon::HttpResponse::newHttpResponse();
            resp->setStatusCode(drogon::k403Forbidden);
            resp->setBody("Origin not allowed.");
            mcb(resp);
            return;
        }

        nextCb([mcb](const HttpResponsePtr &resp) {
            mcb(resp);
        });
    }
};