#include "HeightMap.h"

using namespace osg;

HeightMap::HeightMap(int count, float t_width)
    : types(count + 3, count + 3), heights(count + 3, count + 3), tile_width(t_width),
      sample_spacing(tile_width / (float)(count))
{
    //Initialise types
    types.init(TYPE_WATER);
    heights.init(0.0f);
}

/** Set properties for a sample */
void HeightMap::set(int x, int y, float height, bool water)
{
    heights.get(x + 1, y + 1) = height;
    types.get(x + 1, y + 1) = water ? TYPE_WATER : TYPE_ICE;
}

osg::Vec3 HeightMap::getVertex(int x, int y) const
{
    return osg::Vec3(sample_spacing * x, sample_spacing * y, getHeight(x, y));
}

osg::Vec3 HeightMap::getNormal(int x, int y) const
{
    /*
     * Normal of a point can be found by considering the average
     * Of normals of triangles against it
     */
    Vec3 here = getVertex(x, y);

#ifdef SIMPLE_HEIGHT_MAP_NORMALS
    //Vectors of NESW compass points direction from here to next vertex
    Vec3 n = getVertexp(x, y + 1) - here;
    Vec3 e = getVertexp(x + 1, y) - here;
    Vec3 s = getVertexp(x, y - 1) - here;
    Vec3 w = getVertexp(x - 1, y) - here;

    //Normals of adjacent triangles by compass ref
    Vec3 ne = (e ^ n);
    Vec3 se = (s ^ e);
    Vec3 sw = (w ^ s);
    Vec3 nw = (n ^ w);

    ne.normalize();
    se.normalize();
    sw.normalize();
    nw.normalize();

    Vec3 norm = (ne + se + sw + nw);
    norm.normalize();
#else
    //Vectors of compass points in direction from here to next vertex
    //(Vectors are in order, clockwise (i.e. N, NE, E, SE, S, SW, W, NW)
    //First find the vertex coords of the vertex in each direction
    Vec3 dirs[8] = {
        getVertex(x, y + 1), getVertex(x + 1, y + 1),
        getVertex(x + 1, y), getVertex(x + 1, y - 1),
        getVertex(x, y - 1), getVertex(x - 1, y - 1),
        getVertex(x - 1, y), getVertex(x - 1, y + 1)
    };

    Vec3 norm(0,0,0);

    for (int i = 0; i < 8; i++) {
        //Find the normalised direction vector for the vertex in this direction
        dirs[i] -= here;
        dirs[i].normalize();

        //Compute the normal for the octent defined by this direction and
        //the one one-counter-clockwise turn away
        if (i > 0) {
            Vec3 octentNorm = dirs[i] ^ dirs[i - 1];
            norm += octentNorm;
        }
    }

    norm.normalize();
#endif

    return norm;
}

int HeightMap::getWidth() const {
    return types.width() - 3;
}

bool HeightMap::isIce(int x, int y) const {
    return types.get(x + 1, y + 1) == TYPE_ICE || types.get(x + 1, y + 1) == TYPE_BOUNDARY;
}

bool HeightMap::isWater(int x, int y) const {
    return types.get(x + 1, y + 1) == TYPE_WATER || types.get(x + 1, y + 1) == TYPE_BOUNDARY;
}

float HeightMap::getSampleSpacing() const {
    return sample_spacing;
}

/** Scan the samples and find ice-water boundaries **/
void HeightMap::calcBoundaries()
{
    for (int x = 0; x < types.width(); x++) {
        for (int y = 0; y < types.height(); y++) {
            HType t = types.get(x, y);

            if (testCompareBoundary(x + 1, y, t) ||
                    testCompareBoundary(x - 1, y, t) ||
                    testCompareBoundary(x, y + 1, t) ||
                    testCompareBoundary(x, y - 1, t)) {
                types.get(x, y) = TYPE_BOUNDARY;
            }
        }
    }
}

/**
 * @brief Test whether the location is different to the expected type
 * @return True if exists at this location and is not the expected type
 */
bool HeightMap::testCompareBoundary(int x, int y, HType expectedType)
{
    if (x < 0 || x >= types.width() || y < 0 || y >= types.height())
        return false;

    if (types.get(x, y) == TYPE_BOUNDARY || expectedType != TYPE_ICE)
        return false;

    return types.get(x, y) != expectedType;
}

HeightMapSimplifier::HeightMapSimplifier(IHeightMap* o, int factor)
    : other(o), div(factor)
{
}

osg::Vec3 HeightMapSimplifier::getVertex(int x, int y) const
{
    return other->getVertex(x*div, y*div);
}

osg::Vec3 HeightMapSimplifier::getNormal(int x, int y) const
{
    return other->getNormal(x*div, y*div);
}

int HeightMapSimplifier::getWidth() const
{
    return other->getWidth() / div;
}

bool HeightMapSimplifier::isIce(int x, int y) const
{
    return other->isIce(x*div, y*div);
}

bool HeightMapSimplifier::isWater(int x, int y) const
{
    return other->isWater(x*div, y*div);
}

float HeightMapSimplifier::getSampleSpacing() const
{
    return other->getSampleSpacing() * (float)div;
}
