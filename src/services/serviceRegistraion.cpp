#include "serviceRegistraion.hpp"
#include <iostream>
#include "../dto/userDtro.hpp"
#include "../models/User.hpp"
#include "../repositories/UserTableRepos.hpp"


void serviceRegistraion(const User &userDto) {
    std::clog << "log serviceRegistraion" << std::endl;
    try {
        User user(userDto.getUsername(), userDto.getHashPassword_(), userDto.getEmail());



    } catch (...) {
        std::clog << "err serviceRegistraion" << std::endl;
        throw;
    }
}