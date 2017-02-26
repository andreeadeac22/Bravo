#ifndef TILELOADER_H
#define TILELOADER_H

#include <cstdint>
#include <string>

#include <pqxx/pqxx>

#define TILE_SIZE 512

class SquareTile {
 public:
    SquareTile(const char* data, const size_t length);

    float getHeightAt(int x, int y);

 private:
    std::uint16_t raw[TILE_SIZE][TILE_SIZE];
};

class TileLoader {
 public:
    virtual SquareTile* getTileAt(int x, int y) = 0;
};

class PGTileLoader : TileLoader {
 public:
    PGTileLoader(const std::string dbname);

    ~PGTileLoader();

    virtual SquareTile* getTileAt(int x, int y);

 private:
    pqxx::connection *conn;
};

#endif  // TILELOADER_H
