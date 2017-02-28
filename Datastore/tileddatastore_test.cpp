#include <pqxx/pqxx>
#include <iostream>

#include <snappy.h>
#include "TileStore.h"

int main() {
    TileStore ts("grpproj");
    SquareTile tile = ts.getTileById(0);
    std::cout << "Retrieved tile with id " << tile.id << std::endl;
}
