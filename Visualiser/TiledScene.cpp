#include "TiledScene.h"

#include "util/log.h"

using namespace osg;

/**
 * @brief Construct a tiled scene, specifying the layout of the tiles
 * @param tileTypes Description of tile layout. Each entry in
 * the tileTypes grid indicates what type of tile to load
 * at this point (see TerrainTile::TileType)
 * @param t_width    Width of each tile (in units/meters)
 */
TiledScene::TiledScene(Array2D<TerrainTile::TileType> &tileTypes, int t_width, std::__cxx11::string dbname)
    : terrainTiles(tileTypes.width(), tileTypes.height()), tile_width((float)t_width),
      group(new Group()), asyncTerrainUpdater(1)
{
    for (int x = 0; x < terrainTiles.width(); x++) {
        for (int y = 0; y < terrainTiles.height(); y++) {
            TerrainTile* tile = constructTerrainTileType(tileTypes.get(x, y), tile_width, Vec2s(x, y));

            terrainTiles.get(x, y) = tile;
            tile->setPosition(Vec3(x * tile_width, y * tile_width, 0.0f));
            group->addChild(tile->getNode());
        }
    }

    asyncTerrainLoader = new AsyncTerrainLoader(Vec2i(tileTypes.width(), tileTypes.height()), *this, t_width, dbname);
}

TiledScene::~TiledScene()
{
    for (int x = 0; x < terrainTiles.width(); x++) {
        for (int y = 0; y < terrainTiles.height(); y++) {
            delete terrainTiles.get(x, y);
        }
    }

    delete asyncTerrainLoader;
}

/**
 * @brief Update the visible nodes based on camera position
 * @param pos
 */
void TiledScene::updateCameraPosition(osg::Vec3d pos)
{
    for (int x = 0; x < terrainTiles.width(); x++) {
        for (int y = 0; y < terrainTiles.height(); y++) {
            Vec3d tileCoord((x + 0.5) * tile_width, (y + 0.5) * tile_width, 0.0f);
            float dist = (pos - tileCoord).length();

            TerrainTile* tile = terrainTiles.get(x, y);
            tile->updateEyeDist(dist);
        }
    }

    Vec2i center(pos.x(), pos.y());
    asyncTerrainLoader->setCurrentCenter(center);
}


void TiledScene::updateTileAt(osg::Vec2i p, HeightMap* heightMap)
{
    TerrainTile* tileAt = terrainTiles.get(p.x(), p.y());
    asyncTerrainUpdater.enqueueJob({tileAt, heightMap});
}
