#pragma once

#include <libpq-fe.h>
#include <iostream>
#include <stdexcept>
#include <vector>

class PSQL {
public:
    static PSQL &getInstance(const char *conninfo = "host=localhost dbname=test user=postgres password=secret") {
        static PSQL instance(conninfo);
        return instance;
    }

private:
    PSQL(const char *conninfo);

public:
    ~PSQL();

    PGresult *execQuery(const char *query);

    PGresult *execQueryParams(const char *query, int nParams, const char **params);

    void createTable(const char *table_name, const char *create_table_query_text);

    void insertData(const std::vector<const char *> &insert_query);

private:
    const char *conninfo_;
    PGconn *conn_;
    PGresult *res_;
};