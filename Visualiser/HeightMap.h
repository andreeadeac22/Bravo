#pragma once

#include <osg/Vec3>

#include <util/Array2d.h>
#include <cstdint>

class HeightMap
{
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

    osg::Vec3 getVertexp(int x, int y) const;

    osg::Vec3 getNormal(int x, int y) const;

    int getWidth() const {
        return types.width() - 3;
    }

    bool isIce(int x, int y) const {
        return types.get(x + 1, y + 1) == TYPE_ICE || types.get(x + 1, y + 1) == TYPE_BOUNDARY;
    }

    bool isWater(int x, int y) const {
        return types.get(x + 1, y + 1) == TYPE_WATER || types.get(x + 1, y + 1) == TYPE_BOUNDARY;
    }

    float getSampleSpacing() const {
        return sample_spacing;
    }

    /** Scan the samples and find ice-water boundaries **/
    virtual void calcBoundaries();

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
