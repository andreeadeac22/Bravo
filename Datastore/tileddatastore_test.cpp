#include <pqxx/pqxx>
#include <iostream>

#include <snappy.h>

int main() {
    pqxx::connection* conn;
    try {
        conn = new pqxx::connection("dbname=grpproj");
        std::cout << "Connected to " << conn->dbname() << std::endl;
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    pqxx::work w(*conn);
    pqxx::result r = w.exec("select col, row, data from data where id=1");

    w.commit();

    pqxx::binarystring bs(r[0][2]);

    std::cout << "bs.size() = " << bs.size() << std::endl;

    std::string rawbs;
    bool success = snappy::Uncompress(bs.get(), bs.size(), &rawbs);

    if (!success) {
        std::cerr << "Decompressing tile data failed." << std::endl;
        return 1;
    }


    return 0;
}