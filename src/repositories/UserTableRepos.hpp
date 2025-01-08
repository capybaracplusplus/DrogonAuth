#pragma once

#include "../models/User.hpp"

class UserRepos {
    using id = size_t;
    using Connect = int;
public:

    UserRepos(const Connect &);

public:
    void create_user(const User&);

    void delete_user(const User&);

    User read_user(id);

    void update_user(const User&);

private:
    Connect connect;
};
