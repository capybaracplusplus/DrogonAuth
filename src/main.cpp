#include <iostream>
#include "drogon/drogon.h"
#include "middlewares/AuthMiddleware.hpp"
#include "controllers/AuthController.h"

/*
 "redis_clients": [
    {
      "host": "127.0.0.1",
      "port": 6379,
      "passwd": "",
      "db": 0,
      "is_fast": false,
      "number_of_connections": 1,
      "timeout": -1.0
    }
  ],
*/


int main() {
    std::clog << "log start" << std::endl;
    std::clog << "Drogon version: " << drogon::getVersion() << std::endl;

    drogon::app().loadConfigFile("../src/config.json");
    std::clog << "log loadConfigFile" << std::endl;

    //drogon::app().createRedisClient("127.0.0.1", 6379);

    drogon::app().run();

    return 0;
}
