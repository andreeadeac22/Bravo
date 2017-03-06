#pragma once

#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Vec3>
#include <osg/PositionAttitudeTransform>

#include "HeightMap.h"

/**
 * @brief Handles scene representation of a terrain tile in the scene.
 * This is an abstract class as a base for tile types.
 */
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

    TerrainTile(float t_width, osg::Vec2s coords);

    /**
     * @brief Set the position of the origin of this tile
     */
    void setPosition(osg::Vec3d pos);

    virtual void show();
    virtual void hide();

    /**
     * @brief Update the distance to the eye
     * @param dist
     */
    virtual void updateEyeDist(float dist) { }

    virtual void setHeightMap(HeightMap *heightMap) {
        throw "This terrain tile does not accept height maps";
    }

    virtual ~TerrainTile() = default;

    osg::Node* getNode() {
        return tile_position.get();
    }

    osg::Vec2s getCoords() const {
        return tile_coords;
    }

    float getTileWidth() const {
        return tile_width;
    }

    bool isGenerated() const {
        return generated;
    }

    void setGenerated() {
        generated = true;
    }

protected:
    osg::ref_ptr<osg::PositionAttitudeTransform> tile_position;
    osg::ref_ptr<osg::Geode> tile_geode;

    const float tile_width;

    const osg::Vec2s tile_coords;

    bool generated;

    /**
     * @brief Set the geometry for this tile (replaces current set geometry)
     */
    virtual void setModel(osg::ref_ptr<osg::Geometry> geom);

};

class WaterTile : public TerrainTile
{
public:
    WaterTile(float t_width, osg::Vec2s coords);

private:
    osg::ref_ptr<osg::Geometry> geometry;

};

class BoringIceTile : public TerrainTile
{
public:
    BoringIceTile(float t_width, osg::Vec2s coords);

private:
    osg::ref_ptr<osg::Geometry> geometry;

};

class StaticBoundaryIceTile : public TerrainTile
{
public:
    StaticBoundaryIceTile(float t_width, osg::Vec2s coords);

    virtual void setHeightMap(HeightMap *heightMap);
};

class StaticIceTile : public TerrainTile
{
public:
    StaticIceTile(float t_width, osg::Vec2s coords);

    virtual void setHeightMap(HeightMap *heightMap);

    virtual void updateEyeDist(float dist);

private:
    osg::ref_ptr<osg::Geometry> geomFull;
    osg::ref_ptr<osg::Geometry> geomReduced;

    void render(IHeightMap *heightMap, osg::Geometry* geometry);

};

/**
 * @brief Construct a terrain tile of given type
 */
TerrainTile* constructTerrainTileType(TerrainTile::TileType type, float t_width, osg::Vec2s coords);
