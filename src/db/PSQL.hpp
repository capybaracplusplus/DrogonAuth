#pragma once

#include <libpq-fe.h>
#include <iostream>
#include <stdexcept>
#include <vector>

class PSQL {
public:
    PSQL(const char *conninfo);

    ~PSQL();

    PGresult *execQuery(const char *query);

    void createTable(const char *table_name, const char *create_table_query_text);

    void insertData(const std::vector<const char *> &insert_query);

private:
    const char *conninfo_;
    PGconn *conn_;
    PGresult *res_;
};