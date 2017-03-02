#pragma once

#include <vector>

#include <osg/Vec2>

#include "util/Array2d.h"

struct CrackSegment {
    osg::Vec2 apos;
    float awidth;
    float adepth;

    osg::Vec2 bpos;
    float bwidth;
    float bdepth;

    bool isInsideSeg(osg::Vec2 pos) const;

    /**
     * @brief Get the direction 90 degrees to this segment
     * @return
     */
    osg::Vec2 getSideways() const;

    float getRelativeAlongLine(osg::Vec2 pos) const;

    float getAngleFromCenter(osg::Vec2 pos) const;

    /** Using a point near the edge, find the nearest point ON the edge.*/
    osg::Vec2 getNearestPointOnEdge(osg::Vec2 pos) const;

    osg::Vec2 getNearestPointOnLineSeg(osg::Vec2 a, osg::Vec2 b, osg::Vec2 pos) const;
};

/**
 * @brief Crack segment axis aligned bounding box (also integer aligned)
 */
class CrackSegAABB {
//    struct AdjacentVertex {
//        osg::Vec2 pos;

//        AdjacentVertex* left;
//        AdjacentVertex* right;
//    };

public:
    CrackSegAABB(CrackSegment seg);

    bool getSkipFace(int x, int y);

    //Get a list of vertices surrounding this seg
    std::vector<osg::Vec2s>* getEncirclingVertices();

private:
    osg::Vec2s offset;
    osg::Vec2s size;

    Array2D<bool> *hmapFaceOccl;

};
