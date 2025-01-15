cmake_minimum_required(VERSION 3.30)
project(DrogonAuth)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(libs/Bcrypt.cpp)
add_subdirectory(libs/redis-plus-plus)
include_directories(/path/to/redis-plus-plus)

find_package(Drogon REQUIRED)
find_package(PostgreSQL REQUIRED)
find_package(nlohmann_json REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/libs/jwt-cpp/include)


#include_directories(${CMAKE_SOURCE_DIR}/libs/libsodium-cmake/libsodium/src/libsodium/include)


add_executable(my_drogon_app
        src/main.cpp
        src/models/user.hpp
        src/dto/userDtro.hpp
        src/middlewares/ValidateRequestBodyMiddleware.hpp
        src/controllers/authController.cpp
        src/controllers/authController.h
        src/repositories/userRepos.hpp
        src/repositories/userRepos.cpp
        src/services/serviceAuth.cpp
        src/services/serviceAuth.hpp
        src/repositories/sessionRepos.cpp
        src/repositories/sessionRepos.hpp
        src/utils/jwt/jwtToken.hpp
        src/middlewares/TokenExtractionMiddleware.hpp
        src/middlewares/ValidateTokensMiddleware.hpp
        "src/middlewares/ValidatePasswordMiddleware .hpp"
        "src/middlewares/ValidateEmailAndUsernameMiddleware .hpp"
)

# <------------ add hiredis dependency --------------->
find_path(HIREDIS_HEADER hiredis)
target_include_directories(my_drogon_app PUBLIC ${HIREDIS_HEADER})

find_library(HIREDIS_LIB hiredis)
# <------------ add redis-plus-plus dependency -------------->
# NOTE: this should be *sw* NOT *redis++*
find_path(REDIS_PLUS_PLUS_HEADER sw)
find_library(REDIS_PLUS_PLUS_LIB redis++)




target_link_libraries(my_drogon_app
        PRIVATE Drogon::Drogon
        nlohmann_json::nlohmann_json
        PostgreSQL::PostgreSQL
        bcrypt
        ${HIREDIS_LIB}
        PRIVATE redis++ hiredis
        PUBLIC ${REDIS_PLUS_PLUS_HEADER}
        ${REDIS_PLUS_PLUS_LIB}
)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(my_drogon_app PRIVATE DEBUG)
endif()
