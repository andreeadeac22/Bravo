#ifndef TERRAINTILE_H
#define TERRAINTILE_H

#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Vec3>
#include <osg/PositionAttitudeTransform>

#include "HeightMap.h"

class TerrainTile
{
public:
    enum TileType {
        TYPE_NONE,
        TYPE_WATER,
        TYPE_BORING_ICE,
        TYPE_STATIC_BOUNDARY_ICE,
        TYPE_STATIC_ICE
    };

    TerrainTile(float t_width);

    /**
     * @brief Set the position of the origin of this tile
     */
    void setPosition(osg::Vec3d pos);

    virtual void setHeightMap(HeightMap *heightMap) {
        throw "This terrain tile does not accept height maps";
    }

    osg::Node* getNode() {
        return tile_position.get();
    }

protected:
    osg::ref_ptr<osg::PositionAttitudeTransform> tile_position;
    osg::ref_ptr<osg::Geode> tile_geode;

    const float tile_width;

};

class WaterTile : public TerrainTile
{
public:
    WaterTile(float t_width);

private:
    osg::ref_ptr<osg::Geometry> geometry;

};

class BoringIceTile : public TerrainTile
{
public:
    BoringIceTile(float t_width);

private:
    osg::ref_ptr<osg::Geometry> geometry;

};

class StaticBoundaryIceTile : public TerrainTile
{
public:
    StaticBoundaryIceTile(float t_width);

    virtual void setHeightMap(HeightMap *heightMap);
};

class StaticIceTile : public TerrainTile
{
public:
    StaticIceTile(float t_width);

    virtual void setHeightMap(HeightMap *heightMap);
};

/**
 * @brief Construct a terrain tile of given type
 */
TerrainTile* constructTerrainTileType(TerrainTile::TileType type, float t_width);

#endif // TERRAINTILE_H
