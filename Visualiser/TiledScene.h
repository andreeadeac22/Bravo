#pragma once

#include <osg/Group>
#include <osg/Referenced>

#include "util/Array2d.h"
#include "TerrainTile.h"
#include "HeightMap.h"
#include "AsyncTerrainUpdater.h"
#include "AsyncTerrainLoader.h"

/**
 * @brief Handles loading/unloading of terrain tiles,
 * including based on the users position in the scene.
 *
 * To use a tiled scene to render the scene, one must
 * construct a tile, specifying the grid of tile types
 * (see constructor). The camera position should be updated
 * every frame (calling updateCameraPosition - which
 * will also processes visible tiles).
 */
class TiledScene : public osg::Referenced
{
public:
    /**
     * @brief Construct a tiled scene, specifying the layout of the tiles
     * @param tileTypes Description of tile layout. Each entry in
     * the tileTypes grid indicates what type of tile to load
     * at this point (see TerrainTile::TileType)
     * @param t_width    Width of each tile (in units/meters)
     */
    TiledScene(Array2D<TerrainTile::TileType> &tileTypes, float t_width);

    ~TiledScene();

    /**
     * @brief Update the visible nodes based on camera position
     * @param pos
     */
    void updateCameraPosition(osg::Vec3d pos);

    void setRenderDistance(float dist) {
        render_distance_sq = dist * dist;
    }

    osg::Node* getNode() {
        return group.get();
    }

    /**
     * @brief Queue an update of the tiles at p with a heightmap
     * @param p
     * @param heightMap
     */
    void updateTileAt(osg::Vec2i p, HeightMap* heightMap);


private:
    Array2D<TerrainTile*> terrainTiles;

    float tile_width;

    osg::ref_ptr<osg::Group> group;

    float render_distance_sq;

    AsyncTerrainUpdater asyncTerrainUpdater;

public:AsyncTerrainLoader *asyncTerrainLoader;

};
