#include <iostream>
#include "drogon/drogon.h"

int main() {
    std::clog << "log start" << std::endl;

    drogon::app().loadConfigFile("config.json");
    auto dbClient = drogon::app().getDbClient();
    drogon::app().run();




    return 0;
}
