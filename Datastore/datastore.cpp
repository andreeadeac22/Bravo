#include "datastore.h"

#include <iostream>

using namespace std;

int Datastore::connect(string dbname) {
    try {
        conn = unique_ptr<pqxx::connection>(
                new pqxx::connection("dbname=" + dbname));
        cout << "Connected to " << conn->dbname() << endl;
    } catch (const exception &e) {
        cerr << e.what() << endl;
        return 1;
    }
    return 0;
}

pqxx::result Datastore::run_query(string sql) {
    pqxx::result res;
    try {
        pqxx::work W(*conn);
        res = W.exec(sql);
        W.commit();
    } catch (const exception &e) {
        cerr << e.what() << endl;
    }
    return res;
}

DbData Datastore::processDbRow(pqxx::result::tuple row) {
    DbData data;
    data.id = row[0].as<int>();
    data.col = row[1].as<int>();
    data.row = row[2].as<int>();
    return data;
}
