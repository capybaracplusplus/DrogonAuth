#include <iostream>
#include "drogon/drogon.h"
#include "middlewares/ValidateTokensMiddleware.hpp"
#include "middlewares/ValidateRequestBodyMiddleware.hpp"
#include "middlewares/ValidateUserRequestMiddleware.hpp"
#include "controllers/authController.h"

namespace repos {
    sw::redis::Redis redisDbClient = sw::redis::Redis("tcp://127.0.0.1:6379");
}

int main() {
    std::clog << "log start" << std::endl;
    std::clog << "Drogon version: " << drogon::getVersion() << std::endl;

    drogon::app().loadConfigFile("../src/config.json");
    std::clog << "log loadConfigFile" << std::endl;

    drogon::app().run();

    return 0;
}
