#include "TileStore.h"

#include <cstring>
#include <iostream>

#include <pqxx/pqxx>
#include <snappy.h>

#define min(x, y) (x < y ? x : y)

SquareTile::SquareTile(int id, int col, int row, int tileVertWidth, const char* data, const size_t length)
    : raw(tileVertWidth, tileVertWidth)
{
    this->id = id;
    this->row = row;
    this->col = col;
    // copy the raw data into our 2d array
    raw.copyFrom(reinterpret_cast<const uint16_t*>(data), length);
}

float SquareTile::getHeightAt(int x, int y) {
    if (x < 0 || x >= raw.height() || y < 0 || y >= raw.width()) {
        // invalid tile position
        std::cerr << "Invalid tile position (" << x << ", " << y << ")."
                << std::endl;

        return 0.0f;
    }

    // TODO(me390): use scaling factors
    //Switch x and y because the raw data loaded in uses the opposite
    //association of x and y to rows and columns
    return static_cast<float>(raw.get(y, x));
}

TileStore::TileStore(const std::string dbspec, int tileVWidth)
    : tileVertWidth(tileVWidth)
{
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
                row[2].as<int>(), tileVertWidth, raw.data(), raw.size());
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
    return nullptr;
}

SquareTile* TileStore::getTileAt(int x, int y) {
    pqxx::work w(*conn);
    pqxx::result r =
            w.prepared("load_tile")(x * tileVertWidth)(y * tileVertWidth).exec();
    if (r.size() == 0)
        return nullptr;
    return processDbRow(r[0]);
}

SquareTile* TileStore::getTileById(int id) {
    pqxx::result r = run_query(
            "select id, col, row, data from data where id="
                    + std::to_string(id));
    return processDbRow(r[0]);
}
