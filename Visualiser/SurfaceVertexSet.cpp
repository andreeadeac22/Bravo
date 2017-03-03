#include "SurfaceVertexSet.h"

#include <algorithm>

using namespace osg;

namespace helper
{
    float absAngle(float a)
    {
        float tau = 2.0f * PI;
        while (a > tau) {
            a -= tau;
        }

        while (a < 0.0) {
            a += tau;
        }
    }

    float angleDiff(float a, float b)
    {
        a = absAngle(a);
        b = absAngle(b);

        float diff = a > b ? a - b : b - a;

        return std::min(diff, 2.0f * PI - diff);
    }

    float angleOf(Vec2 v)
    {
        return atan2(v.y(), v.x());
    }

    Vec2 toVec2(Vec3 v) {
        return Vec2(v.x(), v.y());
    }
}

SurfaceVertexSet::SurfaceVertexSet()
{

}

void SurfaceVertexSet::genSurface(osg::Vec3Array* osgverts, osg::DrawElementsUInt* osgels)
{
    if (vertices.size() < 3)
        return; //wtf

    //Copy the vertices to a new vector with attached element indices
    //Also draw these vertices to the OSG vertex buffer
    std::vector<Vertex> verts(vertices.size());

    for (uint32_t i = 0; i < vertices.size(); i++) {
        verts.push_back(Vertex { vertices[i], i });
        osgverts->push_back(vertices[i]);
    }

    //Stores existing drawn segs
    LineSegVector segsVector;

    //For each vertex, try and draw some tris
    for (uint32_t i = 0; i < vertices.size(); i++) {
        Vec3 curVertex = vertices[i];
        Vec2 curVertex2d(curVertex.x(), curVertex.y());

        //Sort the verts by distance from this vertex
        std::sort(verts.begin(), verts.end(), [curVertex](Vertex a, Vertex b) {
            Vec2 av(a.v.x(), a.v.y());
            Vec2 bv(b.v.x(), b.v.y());
            return (av - curVertex2d).length2() < (bv - curVertex2d).length2();
        });

        Vertex leftVert = verts[1];
        Vertex rightVert = verts[1];

        //Angles are offset slightly by bias so that they pick up the left and right sides
        float leftVertexAngle = helper::angleOf(leftVert - curVertex);
        float leftAngle = leftVertexAngle + 0.0001f;
        float rightAngle = leftVertexAngle - 0.0001f;

        //Try find pairs of vertices to connect vert 0 to.
        for (int idx = 2; idx < verts.size(); idx++) {
            Vertex nxtvert = verts[idx];
            Vec2 nxtvert2d = helper::toVec2(nxtvert.v);

            //Check if can reach the vertex
            if (segsVector.collision(LineSegment(curVertex2d, nxtvert2d)))
                continue;

            //Which of left and right verts to connect to
            float vertAngle = helper::angleOf(nxtvert2d - curVertex2d);
            float leftAngleDiff = helper::angleDiff(leftAngle, vertAngle);
            float rightAngleDiff = helper::angleDiff(rightAngle, vertAngle);
            bool useLeft = leftAngleDiff < rightAngleDiff;

            Vertex thirdVert = useLeft ? leftVert : rightVert;

            //Add a face based on winding order
            if (useLeft) {
                osgels->push_back(i);
                osgels->push_back(thirdVert.id);
                osgels->push_back(nxtvert.id);
            } else {
                osgels->push_back(i);
                osgels->push_back(nxtvert.id);
                osgels->push_back(thirdVert.id);
            }

            //Add line segs to the segsVector to be ignored in future
            Vec2 thirdVert2d = helper::toVec2(thirdVert.v);
            segsVector.add(LineSegment(curVertex2d, thirdVert2d));
            segsVector.add(LineSegment(thirdVert2d, nxtvert2d));
            segsVector.add(LineSegment(curVertex2d, nxtvert2d));

            if (useLeft) {
                leftVert = nxtvert;
                leftAngle = vertAngle;
            } else {
                rightVert = nxtvert;
                rightAngle = vertAngle;
            }
        }
    }
}
