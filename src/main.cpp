#include <iostream>
#include "drogon/drogon.h"
#include "middlewares/AuthMiddleware.hpp"
#include "controllers/AuthController.h"


int main() {
    std::clog << "log start" << std::endl;
    std::clog << "Drogon version: " << drogon::getVersion() << std::endl;

    drogon::app().loadConfigFile("../src/config.json");
    std::clog << "log loadConfigFile" << std::endl;

    drogon::app().run();

    return 0;
}
