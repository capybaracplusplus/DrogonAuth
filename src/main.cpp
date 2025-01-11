#include <iostream>
#include "drogon/drogon.h"
#include "middlewares/AuthMiddleware.hpp"
#include "controllers/AuthController.h"


#include <sw/redis++/redis++.h>


int main() {
    using namespace sw::redis;
    auto redis = Redis("tcp://127.0.0.1:6379");
    redis.set("key", "Hello, Redis!");
    auto val = redis.get("key");
    if (val) {
        // Если ключ существует, вывести значение
        std::cout << "Value: " << *val <<  std::endl;
    } else {
        std::cout << "Key does not exist" <<  std::endl;
    }


    std::clog << "log start" << std::endl;
    std::clog << "Drogon version: " << drogon::getVersion() << std::endl;

    drogon::app().loadConfigFile("../src/config.json");
    std::clog << "log loadConfigFile" << std::endl;

    drogon::app().run();

    return 0;
}
