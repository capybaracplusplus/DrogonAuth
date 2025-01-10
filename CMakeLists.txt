cmake_minimum_required(VERSION 3.30)
project(DrogonAuth)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(libs/Bcrypt.cpp)
include_directories(/path/to/redis-plus-plus)

find_package(Drogon REQUIRED)
find_package(PostgreSQL REQUIRED)
find_package(nlohmann_json REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/libs/jwt-cpp/include)


#include_directories(${CMAKE_SOURCE_DIR}/libs/libsodium-cmake/libsodium/src/libsodium/include)


add_executable(my_drogon_app
        src/main.cpp
        src/models/User.hpp
        src/dto/userDtro.hpp
        src/middlewares/AuthMiddleware.hpp
        src/controllers/AuthController.cc
        src/controllers/AuthController.h
        src/repositories/UserTableRepos.hpp
        src/repositories/UserTableRepos.cpp
        src/services/serviceAuth.cpp
        src/services/serviceAuth.hpp
        src/repositories/redisRepos.cpp
        src/repositories/redisRepos.hpp
)

target_link_libraries(my_drogon_app
        PRIVATE Drogon::Drogon
        nlohmann_json::nlohmann_json
        PostgreSQL::PostgreSQL
        bcrypt
)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(my_drogon_app PRIVATE DEBUG)
endif()
