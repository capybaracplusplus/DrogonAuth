#include <iostream>
#include "drogon/drogon.h"
#include "middlewares/AuthValidationMiddleware.hpp"
#include "controllers/AuthController.h"


int main() {
    std::clog << "log start" << std::endl;
    //auto resp = drogon::HttpResponse::newHttpJsonResponse({{"error", "Empty request body"}});

    std::clog << "Drogon version: " << drogon::getVersion() << std::endl;

    drogon::app().loadConfigFile("../src/config.json");
    std::clog << "log loadConfigFile" << std::endl;

    drogon::app().run();

    return 0;
}
