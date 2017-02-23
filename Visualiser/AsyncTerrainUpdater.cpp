#include "AsyncTerrainUpdater.h"

#include "util/log.h"

#include <glm/gtc/noise.hpp>

/**
 * @brief Function used to generate ice height map randomly, while we dont have real data
 * @param heightMap
 * @param tile_x
 * @param tile_y
 * @param tile_width
 */
static void genIceHeightMap(GridHeightMap &heightMap, int tile_x, int tile_y, float tile_width)
{
    int tileVertexCount = heightMap.getWidth();

    float largeSimplexTileWidth = tile_width * 0.001f;
    float smallSimplexTileWidth = tile_width * 0.01f;

    //Large simplex vertex separation
    float lrgSpxVertSep = largeSimplexTileWidth / (tileVertexCount - 1);

    float lrgSpxOffsetX = tile_x * largeSimplexTileWidth;
    float lrgSpxOffsetY = tile_y * largeSimplexTileWidth;

    float smlSpxVertSep = smallSimplexTileWidth / (tileVertexCount - 1);

    float smlSpxOffsetX = tile_x * smallSimplexTileWidth;
    float smlSpxOffsetY = tile_y * smallSimplexTileWidth;

    for (int x = -1; x < tileVertexCount + 1; x++) {
        for (int y = -1; y < tileVertexCount + 1; y++) {
            glm::vec2 lrgSpxCoord(lrgSpxOffsetX + x * lrgSpxVertSep,
                                  lrgSpxOffsetY + y * lrgSpxVertSep);
            glm::vec2 smlSpxCoord(smlSpxOffsetX + x * smlSpxVertSep,
                                  smlSpxOffsetY + y * smlSpxVertSep);

            float height = 20.0f + glm::simplex(lrgSpxCoord) * 10.0f + glm::simplex(smlSpxCoord) * 2.0f;
            heightMap.set(x, y, height, false);
        }
    }
}

void AsyncTerrainUpdater::process(TerrainTile* tile)
{
    osg::Vec2s tilePos = tile->getCoords();

    GridHeightMap heightMap(32, tile->getTileWidth());
    genIceHeightMap(heightMap, tilePos.x(), tilePos.y(), tile->getTileWidth());
    tile->setHeightMap(&heightMap);
}
