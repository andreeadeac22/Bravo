#ifndef TILESTORE_H
#define TILESTORE_H

#include <cstdint>
#include <string>

#include <pqxx/pqxx>
#include "datastore.h"

#include "util/Array2d.h"

class SquareTile : public DbData{
 public:
    SquareTile(int id, int row, int col, int tileVertWidth, const char* data, const size_t length);

    float getHeightAt(int x, int y);

 private:
    Array2D<uint16_t> raw;
};

class TileStore : Datastore {
public:
    TileStore(const std::string dbname, int tileVWidth);

    SquareTile* processDbRow(pqxx::result::tuple row);

    SquareTile* getTileById(int id);

    SquareTile* getTileAt(int x, int y);

private:
    int tileVertWidth;

};

#endif  // TILESTORE_H
