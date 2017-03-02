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
        TYPE_BOUNDARY,
        TYPE_SKIP
    };

public:
    HeightMap(int count, float t_width);

    virtual osg::Vec3 getVertexp(int x, int y) const = 0;

    virtual osg::Vec3 getNormal(int x, int y) const;

    int getWidth() const {
        return types.width() - 2;
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

    void setSkip(int x, int y) {
        types.get(x, y) = TYPE_SKIP;
    }

    bool getSkip(int x, int y) const {
        return types.get(x, y) == TYPE_SKIP;
    }

    void setShowFace(int x, int y, bool show) {
        showFace.get(x, y) = show;
    }

    bool getShowFace(int x, int y) const {
        return showFace.get(x, y);
    }

    /** Scan the samples and find ice-water boundaries **/
    virtual void calcBoundaries();

protected:
    Array2D<HType> types;

    Array2D<bool> showFace;

    float tile_width;
    float sample_spacing;

    /**
     * @brief Test whether the location is different to the expected type
     * @return True if exists at this location and is not the expected type
     */
    bool testCompareBoundary(int x, int y, HType expectedType);

};

class GridHeightMap : public HeightMap
{
public:
    /**
     * @brief HeightMap
     * @param count Number of samples of height (horizontally)
     * @param t_width    Width of tile
     */
    GridHeightMap(int count, float t_width);

    /** Set properties for a sample */
    void set(int x, int y, float height, bool water);

    float getHeight(int x, int y) const {
        return heights.get(x + 1, y + 1);
    }

    virtual osg::Vec3 getVertexp(int x, int y) const;

private:
    Array2D<float> heights;

};

class VectorHeightMap : public HeightMap
{
public:
    VectorHeightMap(int count, float t_width);

    void set(int x, int y, osg::Vec3 v, bool water, bool boundary);

    virtual osg::Vec3 getVertexp(int x, int y) const;

private:
    Array2D<osg::Vec3> vertices;

};
