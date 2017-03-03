#pragma once

#include <osg/Vec2>

class LineSegment
{
public:
    LineSegment(osg::Vec2 va, osg::Vec2 vb);

    bool intersects(LineSegment other);

private:
    osg::Vec2 a;
    osg::Vec2 b;

};
