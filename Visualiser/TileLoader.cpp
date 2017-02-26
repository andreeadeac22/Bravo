#include "TileLoader.h"

#include <string>
#include <iostream>

#include <pqxx/pqxx>
#include <snappy.h>

#define min(x, y) (x < y ? x : y)

SquareTile::SquareTile(const char* data, const size_t length) {
    // copy the raw data into our 2d array
    memcpy(raw, data, min(sizeof(raw), length));
}

float SquareTile::getHeightAt(int x, int y) {
    if (x < 0 || x >= TILE_SIZE || y < 0 || y >= TILE_SIZE) {
        // invalid tile position
        std::cerr << "Invalid tile position (" << x << ", " << y << ")." << std::endl;

        return 0.0f;
    }

    // TODO(me390): use scaling factors
    return static_cast<float>(raw[x][y]);
}

PGTileLoader::PGTileLoader(const std::string dbspec) {
    conn = new pqxx::connection(dbspec);

    if (conn == NULL) {
        std::cerr << "Unable to create database connection to database " << dbspec << std::endl;
        return;
    } else {
        std::cout << "Successfully created database connection to database " << dbspec << std::endl;
    }

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
