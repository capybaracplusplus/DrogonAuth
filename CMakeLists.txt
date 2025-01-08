cmake_minimum_required(VERSION 3.30)
project(DrogonAuth)

set(CMAKE_CXX_STANDARD 20)

find_package(Drogon REQUIRED)
find_package(PostgreSQL REQUIRED)
find_package(nlohmann_json REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/libs/jwt-cpp/include)


add_executable(my_drogon_app
        src/main.cpp
        src/models/User.hpp
        src/db/PSQL.hpp
        src/db/PSQL.cpp
        src/routes/authRoutes.cpp
        src/routes/authRoutes.hpp
        src/middlewares/AuthMiddleware.hpp
        src/services/serviceRegistraion.hpp
        src/services/serviceRegistraion.cpp
        src/middlewares/UsernameValidation.hpp
        src/dto/userDtro.hpp
        src/repositories/UserTableRepos.hpp
)

target_link_libraries(my_drogon_app
        PRIVATE Drogon::Drogon
        nlohmann_json::nlohmann_json
        PostgreSQL::PostgreSQL
)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(my_drogon_app PRIVATE DEBUG)
endif()
