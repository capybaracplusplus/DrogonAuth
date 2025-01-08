#include <drogon/HttpMiddleware.h>
#include <string>

using namespace drogon;


class EmailMiddleware : public HttpMiddleware<EmailMiddleware> {
public:
    EmailMiddleware() {};// do not omit constructor

    void invoke(const HttpRequestPtr &req,
                MiddlewareNextCallback &&nextCb,
                MiddlewareCallback &&mcb) override;


};