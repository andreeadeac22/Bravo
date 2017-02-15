#include <iostream>
#include <pqxx/pqxx>
#include <datastore.hpp>

int main()
{
    Datastore db;
    db.connect("test_dump");
    pqxx::result res = db.run_query("SELECT id, col, row, surface, thickness, bed, ST_AsText(geom) FROM points");
    std::cout << "Found " << res.size() << " rows." << std::endl;
    vector<DbData> data_all = db.getAllPoints();
    std::cout << "Found " << data_all.size() << " rows." << std::endl;
    vector<DbData> data_rectangle = db.getPointsInRectangle(2592, 1935, 2669, 1976);
    std::cout << "Found " << data_rectangle.size() << " rows." << std::endl;
}