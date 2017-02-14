#include <iostream>
#include <pqxx/pqxx>
#include <datastore.hpp>

using namespace std;

int Datastore::connect(string dbname){
    try
    {
        conn = unique_ptr<pqxx::connection>(new pqxx::connection("dbname="+dbname));
        std::cout << "Connected to " << conn->dbname() << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}

int Datastore::run_query(string sql){
    try
    {
        pqxx::work W(*conn);
        res = W.exec(sql);
        W.commit();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}