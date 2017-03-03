#include "LineSegment.h"

#include "util/Mat2x2.h"

using namespace osg;

LineSegment::LineSegment(osg::Vec2 va, osg::Vec2 vb)
    : a(va), b(vb)
{

}

bool LineSegment::intersects(LineSegment other)
{
    Vec2 adiff = other.a - a;

    //Directions of other and this
    Vec2 odir = other.b - other.a;
    Vec2 tdir = b - a;

    Mat2x2f lhs(tdir.x(), -odir.x(),
                tdir.y(), -odir.y());

    if (!lhs.hasInverse())
        return false;

    Vec2 colt = lhs.inverse().mul(adiff);

    return colt.x() >= 0.0 && colt.x() <= 1.0;
}
