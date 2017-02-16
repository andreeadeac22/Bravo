#ifndef DATASTORE_H
#define DATASTORE_H

#include <iostream>
#include <pqxx/pqxx>
#include <vector>

// Holds data from row in the database.
struct DbData {
    int id;
    int col;
    int row;
    int surface;
    int thickness;
    int bed;
    std::string geom;
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
    
        // Process multiple rows from the database.
        std::vector<DbData> processDbRows(pqxx::result& res);
    
        // Retrieves all points from the database.
        std::vector<DbData> getAllPoints();
    
        // Retrieves all points contained in the rectangle with specified corners.
        std::vector<DbData> getPointsInRectangle(int col1, int row1, int col2, int row2);
};

#endif // DATASTORE_H
