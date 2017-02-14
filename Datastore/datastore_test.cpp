#include <iostream>
#include <pqxx/pqxx>
#include <datastore.hpp>

int main()
{
    Datastore db;
    db.connect("testdb");
    db.run_query("SELECT * FROM test");
    std::cout << "Found " << db.res.size() << " row:" << std::endl;
    for (auto row: db.res)
        std::cout << row[1].c_str() << std::endl;
}