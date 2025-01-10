#pragma once

#include <sw/redis++/redis++.h>

class RedisRepos {
    using refreshToken = std::string;
    using DbClientType = size_t;
public:
    void setToken(const refreshToken &); // создание/установка нового токена

    refreshToken getToken(const refreshToken &); // чтобы проверить, что он существует

    void delToken(); // чтобы можно было выйти из аккаунта и удалить рефреш токен
private:
    DbClientType dbClient_;
};


/*
{
    "ref": "fsdhgsd2314",
    "acc": "agsdf2132a",
}
*/