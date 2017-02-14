using namespace std;

class Datastore {
    public:
        std::unique_ptr<pqxx::connection> conn;
        pqxx::result res;
        int connect(string dbname);
        int run_query(string sql);
};