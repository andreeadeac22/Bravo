#include "TiledScene.h"

#include "util/log.h"

using namespace osg;

/**
 * @brief TiledScene
 * @param tileTypes Description of tile layout
 * @param t_width    Width of each tile (in units/meters)
 */
TiledScene::TiledScene(Array2D<TerrainTile::TileType> &tileTypes, float t_width)
    : terrainTiles(tileTypes.width(), tileTypes.height()), tile_width(t_width),
      group(new Group()), render_distance_sq(10.0f * 10.0f), asyncTerrainUpdater()
{
    for (int x = 0; x < terrainTiles.width(); x++) {
        for (int y = 0; y < terrainTiles.height(); y++) {
            TerrainTile* tile = constructTerrainTileType(tileTypes.get(x, y), tile_width, Vec2s(x, y));

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

/**
 * @brief Update the visible nodes based on camera position
 * @param pos
 */
void TiledScene::updateCameraPosition(osg::Vec3d pos)
{
    pos *= 1.0 / tile_width;

    for (int x = 0; x < terrainTiles.width(); x++) {
        for (int y = 0; y < terrainTiles.height(); y++) {
            float xd = pos.x() - (float)x;
            float yd = pos.y() - (float)y;
            float distFromCenterSq = (float)(xd * xd + yd * yd);

            TerrainTile* tile = terrainTiles.get(x, y);

            if (distFromCenterSq > render_distance_sq) {
                tile->hide();
            } else {
                tile->show();

                if (!tile->isGenerated()) {
                    asyncTerrainUpdater.enqueueUpdate(tile);
                    tile->setGenerated();
                    //log_info << "Queued update of tile " << x << ", " << y << std::endl;
                }
            }
        }
    }
}
