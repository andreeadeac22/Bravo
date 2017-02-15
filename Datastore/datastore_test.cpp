#include <iostream>
#include <pqxx/pqxx>
#include <datastore.hpp>

int main()
{
    Datastore db;
    db.connect("test_dump");
    vector<DbData> data_all = db.getAllPoints();
    std::cout << "Found " << data_all.size() << " rows." << std::endl;
    vector<DbData> data_rectangle = db.getPointsInRectangle(2592, 1935, 2669, 1976);
    std::cout << "Found " << data_rectangle.size() << " rows." << std::endl;
}