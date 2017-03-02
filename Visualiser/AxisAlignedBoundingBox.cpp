#include "AxisAlignedBoundingBox.h"

#include <algorithm>

using namespace osg;

void AxisAlignedBoundingBox2::add(osg::Vec2 p)
{
    //Expand in X;
    ox = std::min(p.x(), ox);
    ex = std::max(p.x(), ex);

    //Expand in Y
    oy = std::min(p.y(), oy);
    ey = std::max(p.y(), ey);
}
