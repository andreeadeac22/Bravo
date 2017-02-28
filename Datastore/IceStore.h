#ifndef ICESTORE_H
#define ICESTORE_H

#include <vector>
#include "datastore.h"

// Holds data from row in the database.
class IceData: public DbData {
public:
    int surface;
    int thickness;
    int bed;
    std::string geom;
};

class IceStore: public Datastore {
public:

    // Process a single row from the database.
    IceData processDbRow(pqxx::result::tuple row);

    std::vector<IceData> processDbRows(pqxx::result& res);

    // Retrieves all points from the database.
    std::vector<IceData> getAllPoints();

    // Retrieves all points contained in the rectangle with specified corners.
    std::vector<IceData> getPointsInRectangle(int col1, int row1, int col2,
            int row2);
};

#endif  // ICESTORE_H
