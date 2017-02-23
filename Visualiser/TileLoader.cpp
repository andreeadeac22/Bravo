#include "TileLoader.h"

#include <string>

#include <pqxx/pqxx>
#include <snappy.h>

#define min(x, y) (x < y ? x : y)

SquareTile::SquareTile(const char* data, const size_t length) {
    // copy the raw data into our 2d array
    memcpy(raw, data, min(sizeof(raw), length));
}

PGTileLoader::PGTileLoader(std::string dbname) {
    conn = new pqxx::connection("dbname="+dbname);

    conn->prepare("load_tile",
        "SELECT col, row, data from data WHERE id=$1");
}

PGTileLoader::~PGTileLoader() {
    delete conn;
}

SquareTile PGTileLoader::getTileAt(int x, int y) {
    int id = (y * TILE_SIZE) + x;

    pqxx::work w(*conn);
    pqxx::result r = w.prepared("load_tile")(id).exec();

    // get raw binary data
    pqxx::binarystring bs(r[0][2]);

    std::string raw;
    snappy::Uncompress(bs.get(), bs.size(), &raw);

    // pass the unstructured bytes to SquareTile
    return SquareTile(raw.data(), raw.size());
}
