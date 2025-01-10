#pragma once

#include "../models/User.hpp"

class AuthService {
public:
    void registration(const User &);

    void login(const User &);

};