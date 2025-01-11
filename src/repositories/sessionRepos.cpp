#include <json/value.h>
#include <json/writer.h>
#include "sessionRepos.hpp"


void Session::upload(const user_id & user_id) {
    Json::Value valueJsonData;
    valueJsonData["accessToken"] = jwtTokens_.accessToken;
    valueJsonData["refreshToken"] = jwtTokens_.refreshToken;
    std::string value = Json::writeString(Json::StreamWriterBuilder(), valueJsonData);
    dbClient_.set(std::to_string(user_id), value);
}

const Session &Session::get(const user_id &) {}

void Session::remove(user_id) {}