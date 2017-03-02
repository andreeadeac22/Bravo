#include "CrackSegment.h"

#include "AxisAlignedBoundingBox.h"

#include <algorithm>

using namespace osg;

bool CrackSegment::isInsideSeg(osg::Vec2 pos) const
{
    osg::Vec2 a2b = bpos - apos;

    osg::Vec2 a2pos = pos - apos;

    //Find the relative distance along this line A->B
    //(of projection of this point - using dot product)
    //Divide by length squared as, need to divide by |a2b| and then divide again
    //to get the relative distance
    float relativeDist = (a2pos * a2b) / a2b.length2();

    //Use relative distance to find the width of crack at this point
    float width = (relativeDist * (bwidth - awidth) + awidth) * 0.5;

    //Find the distance from point to line using dot product with sideways vector
    osg::Vec2 side = getSideways();
    float posDist = fabs((a2pos * side) / side.length());

    return relativeDist >= 0.0 && relativeDist <= 1.0 && posDist < width;
}

/**
 * @brief Get the direction 90 degrees to this segment
 * @return
 */
osg::Vec2 CrackSegment::getSideways() const
{
    //Equivilant of applying 90 degree rotation matrix
    osg::Vec2 a2b = bpos - apos;
    osg::Vec2 side(-1.0 * a2b.y(), a2b.x());
    side.normalize();
    return side;
}

float CrackSegment::getRelativeAlongLine(osg::Vec2 pos) const
{
    osg::Vec2 a2b = bpos - apos;

    osg::Vec2 a2pos = pos - apos;

    //Find the relative distance along this line A->B
    //(of projection of this point - using dot product)
    //Divide by length squared as, need to divide by |a2b| and then divide again
    //to get the relative distance
    float relativeDist = (a2pos * a2b) / a2b.length2();

    if (relativeDist < 0.0) return 0.0;
    if (relativeDist > 1.0) relativeDist = 1.0;

    //Get which side of the line the point is on
    osg::Vec2 side = getSideways();
    return (a2pos * side) < 0.0 ? (-1.0 * relativeDist) : relativeDist;
}

float CrackSegment::getAngleFromCenter(osg::Vec2 pos) const
{
    osg::Vec2 a2b = bpos - apos;
    float anglea2b = atan2(a2b.y(), a2b.x());

    osg::Vec2 center = (bpos - apos) * 0.5 + apos;

    osg::Vec2 pos2center = pos - center;

    return -1.0 * (atan2(pos2center.y(), pos2center.x()) - anglea2b);
}

//Using a point near the edge, find the nearest point ON the edge.
osg::Vec2 CrackSegment::getNearestPointOnEdge(osg::Vec2 pos) const
{
    Vec2 segSide = getSideways() * 0.5;
    Vec2 sal = apos + segSide * awidth;
    Vec2 sar = apos - segSide * awidth;
    Vec2 sbl = bpos + segSide * bwidth;
    Vec2 sbr = bpos - segSide * bwidth;

    std::vector<Vec2> nearestp({
        getNearestPointOnLineSeg(sal, sar, pos),
        getNearestPointOnLineSeg(sar, sbr, pos),
        getNearestPointOnLineSeg(sbr, sbl, pos),
        getNearestPointOnLineSeg(sbl, sal, pos),
    });

    return *std::min_element(nearestp.begin(), nearestp.end(), [pos](Vec2 a, Vec2 b) {
        return (pos - a).length2() < (pos - b).length2();
    });
}

osg::Vec2 CrackSegment::getNearestPointOnLineSeg(osg::Vec2 a, osg::Vec2 b, osg::Vec2 pos) const
{
    Vec2 a2b = b - a;
    Vec2 a2pos = pos - a;

    float relativeDist = (a2pos * a2b) / a2b.length2();

    if (relativeDist < 0.0) relativeDist = 0.0;
    if (relativeDist > 1.0) relativeDist = 1.0;

    return a + a2b * relativeDist;
}

CrackSegAABB::CrackSegAABB(CrackSegment seg)
{
    //Calculate the bounding box
    Vec2 segSide = seg.getSideways() * 0.5;
    Vec2 sal = seg.apos + segSide * seg.awidth;
    Vec2 sar = seg.apos - segSide * seg.awidth;
    Vec2 sbl = seg.bpos + segSide * seg.bwidth;
    Vec2 sbr = seg.bpos - segSide * seg.bwidth;
    AxisAlignedBoundingBox2 aabb(sal);
    aabb.add(sar);
    aabb.add(sbl);
    aabb.add(sbr);

    Vec2 start = aabb.getOrigin();
    Vec2s starts(floor(start.x()), floor(start.y()));
    Vec2 end = aabb.getEnd();
    Vec2s ends(ceil(end.x()) + 1, ceil(end.y()) + 1);

    offset = starts;
    size = ends - starts;

    //Grid of vertices, mapping which are occluded by the segment
    hmapFaceOccl = new Array2D<bool>(size.x(), size.y());

    Array2D<bool> hmapVertexOccl(size.x() + 1, size.y() + 1);

    //Figure out which vertices are occluded
    for (int x = 0; x <= size.x(); x++) {
        for (int y = 0; y <= size.y(); y++) {
            Vec2 vertPos(offset.x() + x, offset.y() + y);
            if (seg.isInsideSeg(vertPos)) {
                hmapVertexOccl.get(x, y) = true;
                log_info << "Seg is inside " << vertPos << std::endl;
            } else {
                hmapVertexOccl.get(x, y) = false;
            }
        }
    }

    //Map occluded vertices onto faces
    for (int x = 0; x < size.x(); x++) {
        for (int y = 0; y < size.y(); y++) {
            bool obl = hmapVertexOccl.get(x, y);
            bool obr = hmapVertexOccl.get(x + 1, y);
            bool otl = hmapVertexOccl.get(x, y + 1);
            bool otr = hmapVertexOccl.get(x + 1, y + 1);

            if (obl || obr || otl || otr) {
                hmapFaceOccl->get(x, y) = true;
            } else {
                hmapFaceOccl->get(x, y) = false;
            }
        }
    }

    Vec2 segVertices[] = { sal, sar, sbl, sbr };
    for (int i = 0; i < 4; i++) {
        Vec2 segVert = segVertices[i];
        int segIdxX = floor(segVert.x()) - offset.x();
        int segIdxY = floor(segVert.y()) - offset.y();

        hmapFaceOccl->get(segIdxX, segIdxY) = true;

        log_info << "Face is inside " << segVert << " - " << segIdxX << ", " << segIdxY << std::endl;
    }
}

bool CrackSegAABB::getSkipFace(int x, int y)
{
    int ox = x - offset.x();
    int oy = y - offset.y();

    if (ox < 0 || ox >= size.x() || oy < 0 || oy >= size.y())
        return false;

    return hmapFaceOccl->get(ox, oy);
}

std::vector<osg::Vec2s>* CrackSegAABB::getEncirclingVertices()
{
    std::vector<osg::Vec2s>* vertices = new std::vector<osg::Vec2s>();

    for (int x = 0; x <= size.x(); x++) {
        for (int y = 0; y <= size.y(); y++) {
            //Empty for bottom left to top right
            bool ebl = x > 0 && y > 0 ? hmapFaceOccl->get(x - 1, y - 1) : false;
            bool ebr = x < size.x() && y > 0 ? hmapFaceOccl->get(x, y - 1) : false;
            bool etl = x > 0 && y < size.y() ? hmapFaceOccl->get(x - 1, y) : false;
            bool etr = x < size.x() && y < size.y() ? hmapFaceOccl->get(x, y) : false;

            //If some are empty but not all.
            if ((ebl || ebr || etl || etr) &&
                    !(ebl && ebr && etl && etr)) {
                vertices->push_back(Vec2s(x, y) + offset);
            }
        }
    }

    return vertices;
}
