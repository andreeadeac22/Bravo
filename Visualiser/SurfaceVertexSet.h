#pragma once

#include <vector>

#include <osg/Vec3>
#include <osg/Drawable>

#include "LineSegment.h"

/**
 * @brief A set of vertices forming a surface.
 */
class SurfaceVertexSet
{
    struct Vertex {
        uint32_t id;
        osg::Vec3 v;
    };

public:
    SurfaceVertexSet();

    void add(osg::Vec3 vertex) {
        vertices.push_back(vertex);
    }

    /**
     * @brief Generate surface from the vertices in this set
     * @param osgverts  Vertex buffer output
     * @param osgels    Element array describing faces
     */
    void genSurface(osg::Vec3Array* osgverts, osg::DrawElementsUInt* osgels);

private:
    std::vector<osg::Vec3> vertices;

};

class LineSegVector {
public:
    void add(LineSegment seg) {
        segs.push_back(seg);
    }

    bool collision(LineSegment seg) {
        for (LineSegment s: segs) {
            if (seg.intersects(s))
                return true;
        }

        return false;
    }

private:
    std::vector<LineSegment> segs;

};

