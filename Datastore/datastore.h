#ifndef DATASTORE_H
#define DATASTORE_H

#include <pqxx/pqxx>
#include <vector>

// Holds data from row in the database.
class DbData {
public:
    int id;
    int col;
    int row;
};

// Class used to store/retrieve data from database.
class Datastore {
public:
    // Connection to current database.
    std::unique_ptr<pqxx::connection> conn;

    // Connects to database with dbname.
    int connect(std::string dbname);

    // Runs a query on the database.
    pqxx::result run_query(std::string sql);

    // Process a single row from the database.
    DbData processDbRow(pqxx::result::tuple row);

};

#endif // DATASTORE_H
