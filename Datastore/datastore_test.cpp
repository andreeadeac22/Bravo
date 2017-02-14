#include <iostream>
#include <pqxx/pqxx>

int main()
{
    try
    {
        pqxx::connection C("dbname=testdb");
        std::cout << "Connected to " << C.dbname() << std::endl;
        pqxx::work W(C);

        pqxx::result R = W.exec("SELECT * FROM test");
        w.commit();

        std::cout << "Found " << R.size() << " row:" << std::endl;
        for (auto row: R)
            std::cout << row[1].c_str() << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}