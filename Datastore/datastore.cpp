#include <iostream>
#include <pqxx/pqxx>
#include <datastore.hpp>

using namespace std;

int Datastore::connect(string dbname) {
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

pqxx::result Datastore::run_query(string sql) {
    pqxx::result res;
    try
    {
        pqxx::work W(*conn);
        res = W.exec(sql);
        W.commit();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    return res;
}

DbData Datastore::processDbRow(pqxx::result::tuple row) {
    DbData data;
    data.id = row[0].as<int>();
    data.col = row[1].as<int>();
    data.row = row[2].as<int>();
    data.surface = row[3].as<int>();
    data.thickness = row[4].as<int>();
    data.bed = row[5].as<int>();
    data.geom = string(row[6].c_str());
    return data;
}

vector<DbData> Datastore::processDbRows(pqxx::result& res) {
    vector<DbData> db_data_vec;
    for(auto row:res){
        db_data_vec.push_back(processDbRow(row));
    }
    return db_data_vec;
}

vector<DbData> Datastore::getAllPoints() {
    pqxx::result res = run_query("SELECT id, col, row, surface, thickness, bed, ST_AsText(geom) FROM points ORDER BY col, row");
    return processDbRows(res);
}

vector<DbData> Datastore::getPointsInRectangle(int col1, int row1, int col2, int row2){
    pqxx::result res = run_query("SELECT id, col, row, surface, thickness, bed, ST_AsText(geom) FROM points WHERE ST_MakeEnvelope(" + to_string(col1) + "," + to_string(row1) + "," + to_string(col2) + "," + to_string(row2) + ", 4326)&& geom ORDER BY col, row");
    return processDbRows(res);
}