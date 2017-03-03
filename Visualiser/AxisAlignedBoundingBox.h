#pragma once
#include <osg/Vec2>

/**
 * @brief 2 dimensional axis aligned bounding box
 * Can repeatedly add points and bounding box will expand
 * to fit all points
 */
class AxisAlignedBoundingBox2
{
public:
    /**
     * @brief Construct with one point (and zero size)
     * @param origin
     */
    AxisAlignedBoundingBox2(osg::Vec2 origin)
        : ox(origin.x()), oy(origin.y()), ex(origin.x()), ey(origin.y()) { }

    void add(osg::Vec2 p);

    osg::Vec2 getOrigin() const {
        return osg::Vec2(ox, oy);
    }

    osg::Vec2 getEnd() const {
        return osg::Vec2(ex, ey);
    }

private:
    float ox;
    float oy;
    float ex;
    float ey;

};
