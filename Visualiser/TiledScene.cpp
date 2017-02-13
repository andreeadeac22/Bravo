#include "TiledScene.h"

using namespace osg;

/**
 * @brief TiledScene
 * @param tileTypes Description of tile layout
 * @param t_width    Width of each tile (in units/meters)
 */
TiledScene::TiledScene(Array2D<TerrainTile::TileType> &tileTypes, float t_width)
    : terrainTiles(tileTypes.width(), tileTypes.height()), tile_width(t_width),
      group(new Group())
{
    for (int x = 0; x < terrainTiles.width(); x++) {
        for (int y = 0; y < terrainTiles.height(); y++) {
            TerrainTile* tile = constructTerrainTileType(tileTypes.get(x, y), tile_width);

            terrainTiles.get(x, y) = tile;
            tile->setPosition(Vec3(x * tile_width, y * tile_width, 0.0f));
            group->addChild(tile->getNode());
        }
    }
}

TiledScene::~TiledScene()
{
    for (int x = 0; x < terrainTiles.width(); x++) {
        for (int y = 0; y < terrainTiles.height(); y++) {
            delete terrainTiles.get(x, y);
        }
    }
}

/** Set the height map data for the tile at this location */
void TiledScene::setHeightMap(int x, int y, HeightMap *heightMap)
{
    terrainTiles.get(x, y)->setHeightMap(heightMap);
}
