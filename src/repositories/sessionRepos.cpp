#include <json/value.h>
#include <json/writer.h>
#include <json/reader.h>
#include "sessionRepos.hpp"

using namespace repos;

void Session::upload(const user_id &user_id) {
    Json::Value valueJsonData;
    valueJsonData["accessToken"] = jwtTokens_.accessToken;
    valueJsonData["refreshToken"] = jwtTokens_.refreshToken;
    std::string value = Json::writeString(Json::StreamWriterBuilder(), valueJsonData);
    std::string redisKey = std::to_string(user_id) + ":" + jwtTokens_.accessToken;
    dbClient_.set(redisKey, value);
}

const Session::JwtTokens &Session::get(const user_id &user_id, const JwtTokens &jwtTokens) {
    std::string redisKey = std::to_string(user_id) + ":" + jwtTokens.accessToken;
    auto valueOpt = dbClient_.get(redisKey);
    if (!valueOpt) {
        throw std::runtime_error("user ID not found in Redis");
    }
    Json::Value valueJsonData;
    Json::CharReaderBuilder readerBuilder;
    std::string errs;
    std::istringstream valueStream(*valueOpt);
    if (!Json::parseFromStream(readerBuilder, valueStream, &valueJsonData, &errs)) {
        throw std::runtime_error("Failed to parse JSON from Redis: " + errs);
    }
    jwtTokens_.accessToken = valueJsonData["accessToken"].asString();
    jwtTokens_.refreshToken = valueJsonData["refreshToken"].asString();

    return jwtTokens_;
}

void Session::remove(const user_id &user_id, const JwtTokens &jwtTokens) {
    std::string redisKey = std::to_string(user_id) + ":" + jwtTokens.accessToken;
    auto result = dbClient_.del(redisKey);
    if (result == 0) {
        throw std::runtime_error("Failed to remove user from Redis: Key not found");
    }
}