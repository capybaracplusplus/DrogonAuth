cmake_minimum_required(VERSION 3.30)
project(DrogonAuth)

set(CMAKE_CXX_STANDARD 20)

find_package(Drogon REQUIRED)
find_package(PostgreSQL REQUIRED)
find_package(nlohmann_json REQUIRED)

add_executable(my_drogon_app
        src/main.cpp
)

target_link_libraries(my_drogon_app
        PRIVATE Drogon::Drogon
        nlohmann_json::nlohmann_json
        PostgreSQL::PostgreSQL
)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(my_drogon_app PRIVATE DEBUG)
endif()
