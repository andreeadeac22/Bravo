#include "AsyncTerrainUpdater.h"

#include "util/log.h"
#include "TileLoader.h"

#include <iostream>
#include <glm/gtc/noise.hpp>

/**
 * @brief Function used to generate ice height map randomly, while we dont have real data
 * @param heightMap
 * @param tile_x
 * @param tile_y
 * @param tile_width
 */
static void genIceHeightMap(GridHeightMap &heightMap, int tile_x, int tile_y, float tile_width) {
    std::string dbspec("dbname=grpproj");
    PGTileLoader tl(dbspec);

    SquareTile *t = tl.getTileAt(0, 0);

    std::cout << "tile_x " << tile_x << ", tile_y " << tile_y << std::endl;

    std::cout << "width: " << heightMap.getWidth() << std::endl;

    int width = heightMap.getWidth();

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < width; y++) {
            float height = t->getHeightAt(x, y);
            heightMap.set(x, y, height, false);
        }
    }

    delete t;
}

void TerrainUpdater::process(TerrainTile* tile)
{
    osg::Vec2s tilePos = tile->getCoords();

    GridHeightMap heightMap(32, tile->getTileWidth());
    genIceHeightMap(heightMap, tilePos.x(), tilePos.y(), tile->getTileWidth());
    tile->setHeightMap(&heightMap);
}
