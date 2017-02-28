#include "IceStore.h"
#include <iostream>

using namespace std;

int main() {
    IceStore db;
    db.connect("test_dump");
    // Test raw sql query for retrieving all points.
    pqxx::result res =
            db.run_query(
                    "SELECT id, col, row, surface, thickness, bed, ST_AsText(geom) FROM points");
    std::cout << "Found " << res.size() << " rows." << std::endl;
    // Test retrieving all points.
    vector<IceData> data_all = db.getAllPoints();
    std::cout << "Found " << data_all.size() << " rows." << std::endl;
    // Test retrieving all points inside rectangle.
    vector<IceData> data_rectangle = db.getPointsInRectangle(2592, 1935, 2669,
            1976);
    std::cout << "Found " << data_rectangle.size() << " rows." << std::endl;
}
