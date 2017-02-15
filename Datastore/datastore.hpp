#ifndef DATASTORE_H
#define DATASTORE_H

using namespace std;

// Holds data from row in the database.
struct DbData {
    int id;
    int col;
    int row;
    int surface;
    int thickness;
    int bed;
    string geom;
};

// Class used to store/retrieve data from database.
class Datastore {
    public:
        // Connection to current database.
        std::unique_ptr<pqxx::connection> conn;
        
        // Connects to database with dbname.
        int connect(string dbname);
    
        // Runs a query on the database.
        pqxx::result run_query(string sql);
    
        // Process a single row from the database.
        DbData processDbRow(pqxx::result::tuple row);
    
        // Process multiple rows from the database.
        vector<DbData> processDbRows(pqxx::result& res);
    
        // Retrieves all points from the database.
        vector<DbData> getAllPoints();
    
        // Retrieves all points contained in the rectangle with specified corners.
        vector<DbData> getPointsInRectangle(int col1, int row1, int col2, int row2);
};

#endif // DATASTORE_H
