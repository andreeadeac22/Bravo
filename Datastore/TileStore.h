#ifndef TILESTORE_H
#define TILESTORE_H

#include <cstdint>
#include <string>

#include <pqxx/pqxx>
#include "datastore.h"

#define TILE_SIZE 512

class SquareTile : public DbData{
 public:
    SquareTile(int id, int row, int col, const char* data, const size_t length);

    float getHeightAt(int x, int y);

 private:
    std::uint16_t raw[TILE_SIZE][TILE_SIZE];
};

class TileStore : Datastore {
 public:
    TileStore(const std::string dbname);

    SquareTile* processDbRow(pqxx::result::tuple row);

    SquareTile* getTileById(int id);

    SquareTile* getTileAt(int x, int y);
};

#endif  // TILESTORE_H
