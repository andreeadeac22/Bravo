#ifndef TILEDSCENE_H
#define TILEDSCENE_H

#include <osg/Group>
#include <osg/Referenced>

#include "util/Array2d.h"
#include "TerrainTile.h"
#include "HeightMap.h"

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

    osg::Node* getNode() {
        return group.get();
    }


private:
    Array2D<TerrainTile*> terrainTiles;

    float tile_width;

    osg::ref_ptr<osg::Group> group;

};

#endif // TILEDSCENE_H
