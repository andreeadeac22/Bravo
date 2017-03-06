#include "TileStore.h"

#include <cstring>
#include <iostream>

#include <pqxx/pqxx>
#include <snappy.h>

#define min(x, y) (x < y ? x : y)

SquareTile::SquareTile(int id, int col, int row, const char* data,
        const size_t length) {
    this->id = id;
    this->row = row;
    this->col = col;
    // copy the raw data into our 2d array
    memcpy(raw, data, min(sizeof(raw), length));
}

float SquareTile::getHeightAt(int x, int y) {
    if (x < 0 || x >= TILE_SIZE || y < 0 || y >= TILE_SIZE) {
        // invalid tile position
        std::cerr << "Invalid tile position (" << x << ", " << y << ")."
                << std::endl;

        return 0.0f;
    }

    // TODO(me390): use scaling factors
    return static_cast<float>(raw[x][y]);
}

TileStore::TileStore(const std::string dbspec) {
    connect(dbspec);
    conn->prepare("load_tile",
            "SELECT id, col, row, data from data WHERE col=$1 AND row=$2");
}

SquareTile* TileStore::processDbRow(pqxx::result::tuple row) {
    // get raw binary data
    try {
        pqxx::binarystring bs(row[3]);
        std::string raw;
        snappy::Uncompress(bs.get(), bs.size(), &raw);
        return new SquareTile(row[0].as<int>(), row[1].as<int>(),
                row[2].as<int>(), raw.data(), raw.size());
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return nullptr;
}

SquareTile* TileStore::getTileAt(int x, int y) {
    pqxx::work w(*conn);
    pqxx::result r =
            w.prepared("load_tile")(x * TILE_SIZE)(y * TILE_SIZE).exec();
    return processDbRow(r[0]);
}

SquareTile* TileStore::getTileById(int id) {
    pqxx::result r = run_query(
            "select id, col, row, data from data where id="
                    + std::to_string(id));
    return processDbRow(r[0]);
}
