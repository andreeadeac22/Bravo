#pragma once

#include <osg/Vec3>

#include <util/Array2d.h>
#include <cstdint>

/**
 * @brief Interface for height map objects
 */
class IHeightMap {
public:
    virtual ~IHeightMap() {}

    virtual osg::Vec3 getVertexp(int x, int y) const = 0;

    virtual osg::Vec3 getNormal(int x, int y) const = 0;

    virtual int getWidth() const = 0;

    virtual bool isIce(int x, int y) const = 0;

    virtual bool isWater(int x, int y) const = 0;

    virtual float getSampleSpacing() const = 0;
};

/**
 * @brief HeightMap class which stores the heights of vertices in a grid.
 * Stores height map of a certain width, but also stores heights in adjacent
 * tiles, in order to calculate correct normals
 */
class HeightMap : public IHeightMap {
protected:
    enum HType {
        TYPE_ICE,
        TYPE_WATER,
        TYPE_BOUNDARY
    };

public:
    HeightMap(int count, float t_width);

    /** Set properties for a sample */
    void set(int x, int y, float height, bool water);

    float getHeight(int x, int y) const {
        return heights.get(x + 1, y + 1);
    }

    virtual osg::Vec3 getVertexp(int x, int y) const;

    virtual osg::Vec3 getNormal(int x, int y) const;

    virtual int getWidth() const;

    virtual bool isIce(int x, int y) const;

    virtual bool isWater(int x, int y) const;

    virtual float getSampleSpacing() const;

    /** Scan the samples and find ice-water boundaries **/
    void calcBoundaries();

    /** Copy data the tile on the left */
    void copyFromLeft(HeightMap* map) {
        copyFrom(map, getWidth() - 1, 0, -1, 0, 1, getWidth());
    }

    /** Copy data the tile on the left */
    void copyFromRight(HeightMap* map) {
        copyFrom(map, 0, 0, getWidth(), 0, 2, getWidth());
    }

    /** Copy data the tile on the bottom  */
    void copyFromBottom(HeightMap* map) {
        copyFrom(map, 0, getWidth() - 1, 0, -1, getWidth(), 1);
    }

    /** Copy data the tile on the top  */
    void copyFromTop(HeightMap* map) {
        copyFrom(map, 0, 0, 0, getWidth(), getWidth(), 2);
    }

    /** Copy data from the tile on the bottom left */
    void copyFromBottomLeft(HeightMap* map) {
        copyFrom(map, getWidth() - 1, getWidth() - 1, -1, -1, 1, 1);
    }

    /** Copy data from the tile on the bottom right */
    void copyFromBottomRight(HeightMap* map) {
        copyFrom(map, 0, getWidth() - 1, getWidth(), -1, 2, 1);
    }

    /** Copy data from the tile on the top left */
    void copyFromTopLeft(HeightMap* map) {
        copyFrom(map, getWidth() - 1, 0, -1, getWidth(), 1, 2);
    }

    /** Copy data from the tile on the top right */
    void copyFromTopRight(HeightMap* map) {
        copyFrom(map, 0, 0, getWidth(), getWidth(), 2, 2);
    }

protected:
    Array2D<HType> types;
    Array2D<float> heights;

    float tile_width;
    float sample_spacing;

    /**
     * @brief Test whether the location is different to the expected type
     * @return True if exists at this location and is not the expected type
     */
    bool testCompareBoundary(int x, int y, HType expectedType);

    void copyFrom(HeightMap* map, int ox, int oy, int tx, int ty, int w, int h) {
        S_ASSERT(map->getWidth() == getWidth(), "Map widths mismatched");
        types.copyFrom(map->types, ox + 1, oy + 1, tx + 1, ty + 1, w, h);
        heights.copyFrom(map->heights, ox + 1, oy + 1, tx + 1, ty + 1, w, h);
    }

};

/**
 * @brief Decorator/container class to simplify height map data
 */
class HeightMapSimplifier : public IHeightMap {
public:
    /**
     * @brief
     * @param o Height Map to simplify
     * @param factor    Factor (must be power of 2)
     */
    HeightMapSimplifier(IHeightMap* o, int factor);

    virtual osg::Vec3 getVertexp(int x, int y) const;

    virtual osg::Vec3 getNormal(int x, int y) const;

    virtual int getWidth() const;

    virtual bool isIce(int x, int y) const;

    virtual bool isWater(int x, int y) const;

    virtual float getSampleSpacing() const;

private:
    IHeightMap* other;

    int div;

};
