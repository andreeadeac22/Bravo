#ifndef TILEDSCENE_H
#define TILEDSCENE_H

#include <osg/Group>
#include <osg/Referenced>

#include "util/Array2d.h"
#include "TerrainTile.h"
#include "HeightMap.h"
#include "AsyncTerrainUpdater.h"

class TiledScene : public osg::Referenced
{
public:
    /**
     * @brief TiledScene
     * @param tileTypes Description of tile layout
     * @param t_width    Width of each tile (in units/meters)
     */
    TiledScene(Array2D<TerrainTile::TileType> &tileTypes, float t_width);

    ~TiledScene();

    /** Set the height map data for the tile at this location */
    void setHeightMap(int x, int y, HeightMap *heightMap);

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


private:
    Array2D<TerrainTile*> terrainTiles;

    float tile_width;

    osg::ref_ptr<osg::Group> group;

    float render_distance_sq;

    AsyncTerrainUpdater asyncTerrainUpdater;

};

#endif // TILEDSCENE_H
