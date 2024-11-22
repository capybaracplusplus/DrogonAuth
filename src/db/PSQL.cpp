#include "PSQL.hpp"

PSQL::PSQL(const char *conninfo) : conninfo_(conninfo), conn_(nullptr), res_(nullptr) {
    conn_ = PQconnectdb(conninfo_);
    if (PQstatus(conn_) != CONNECTION_OK) {
        std::cerr << "Ошибка подключения: " << PQerrorMessage(conn_) << std::endl;
        PQfinish(conn_);
        throw std::runtime_error("Ошибка подключения к базе данных");
    }
    std::cout << "Успешно подключено к базе данных!" << std::endl;
}

PSQL::~PSQL() {
    if (res_) {
        PQclear(res_);
    }
    PQfinish(conn_);
}

PGresult *PSQL::execQuery(const char *query) {
    res_ = PQexec(conn_, query);
    if (PQresultStatus(res_) != PGRES_TUPLES_OK && PQresultStatus(res_) != PGRES_COMMAND_OK) {
        std::cerr << "Ошибка выполнения запроса: " << PQerrorMessage(conn_) << std::endl;
        PQclear(res_);
        throw std::runtime_error("Ошибка выполнения запроса execQuery");
    }
    return res_;
}

void PSQL::createTable(const char *table_name, const char *create_table_query_text) {
    try {
        std::string createQuery =
                "CREATE TABLE IF NOT EXISTS " + std::string(table_name) + " " + create_table_query_text;
        execQuery(createQuery.c_str());
        std::cout << "Таблица " << table_name << " успешно создана!" << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Ошибка при создании таблицы: " << e.what() << std::endl;
    }
}

void PSQL::insertData(const std::vector<const char *> &insert_query) {
    try {
        for (const auto& elem: insert_query) {
            execQuery(elem);
        }
        std::cout << "Данные успешно вставлены в таблицу!" << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Ошибка при вставке данных: " << e.what() << std::endl;
    }
}

