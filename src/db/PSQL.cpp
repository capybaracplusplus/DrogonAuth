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

PGresult *PSQL::execQueryParams(const char *query, int nParams, const char **params) {
    // Выполняем запрос с параметрами
    PGresult *res = PQexecParams(conn_,
                                 query,          // SQL-запрос
                                 nParams,        // количество параметров
                                 nullptr,        // типы данных параметров (можно указать, если нужно)
                                 params,         // массив значений параметров
                                 nullptr,        // длины параметров (по умолчанию)
                                 nullptr,        // типы параметров (по умолчанию)
                                 0);             // преобразование параметров в строку

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        std::cerr << "Ошибка выполнения запроса: " << PQerrorMessage(conn_) << std::endl;
        PQclear(res);
        throw std::runtime_error("Ошибка выполнения запроса");
    }

    return res;
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
        for (const auto &elem: insert_query) {
            execQuery(elem);
        }
        std::cout << "Данные успешно вставлены в таблицу!" << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Ошибка при вставке данных: " << e.what() << std::endl;
    }
}

