
/**********************
Tutorial 12: The shadow mapping effect in OSG
COpyright (c) Franclin Foping franclin@netcourrier.com
*////////////////////////


#include <osg/Texture2D>

#include <osg/Material>
#include <osg/ShapeDrawable>
#include <osgGA/StateSetManipulator>

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

#include <osgShadow/ShadowedScene>
#include <osgShadow/ShadowMap>

#include <osgDB/ReadFile>

#include <osg/LightSource>

//Simplifier stuff
#include <osg/LOD>
#include <osgUtil/Simplifier>

#include <osgGA/TerrainManipulator>

#include "util/Array2d.h"
#include "util/log.h"

#include "TiledScene.h"
#include "HeightMap.h"
#include "KeyboardController.h"

#include "CrackSegment.h"
#include "LineSegment.h"

#include <iostream>
#include <algorithm>

using namespace osg;

#define TESTER_THINGY

void test_drawSeg(ref_ptr<Group> scene, CrackSegment seg)
{
    ref_ptr<Material> segMaterial = new Material;
    segMaterial->setColorMode(Material::DIFFUSE);
    segMaterial->setAmbient(Material::FRONT_AND_BACK, Vec4(0.1, 0.0, 0.0, 1));
    segMaterial->setDiffuse(Material::FRONT_AND_BACK, Vec4(0.6, 0.0, 0.0, 1.0));
    segMaterial->setSpecular(Material::FRONT_AND_BACK, Vec4(0.8, 0.0, 0.0, 1));
    segMaterial->setShininess(Material::FRONT_AND_BACK, 64);

    Geometry* surface = new Geometry;

    surface->getOrCreateStateSet()->setAttributeAndModes(segMaterial.get(), StateAttribute::ON);

    Vec2 segSide = seg.getSideways() * 0.5;
    Vec2 sal = seg.apos + segSide * seg.awidth;
    Vec2 sar = seg.apos - segSide * seg.awidth;
    Vec2 sbl = seg.bpos + segSide * seg.bwidth;
    Vec2 sbr = seg.bpos - segSide * seg.bwidth;

    Vec3 vbl(sal.x(), sal.y(), 0.5);
    Vec3 vbr(sar.x(), sar.y(), 0.5);
    Vec3 vtl(sbl.x(), sbl.y(), 0.5);
    Vec3 vtr(sbr.x(), sbr.y(), 0.5);

    Vec3Array* triangles = new Vec3Array;
    triangles->push_back(vbl);
    triangles->push_back(vbr);
    triangles->push_back(vtl);
    triangles->push_back(vbr);
    triangles->push_back(vtl);
    triangles->push_back(vtr);

    surface->setVertexArray(triangles);
    surface->addPrimitiveSet(new DrawArrays(PrimitiveSet::TRIANGLES, 0, 6));

    scene->addChild(surface);
}

struct AdjacentVertex {
    Vec2s pos;

    AdjacentVertex* left;
    AdjacentVertex* right;

    Vec2 edgePos;
};

void test_drawEdgeOfCrackSeg(ref_ptr<Group> scene, std::vector<AdjacentVertex>& adjacentVertices)
{
    ref_ptr<Material> segMaterial = new Material;
    segMaterial->setColorMode(Material::DIFFUSE);
    segMaterial->setAmbient(Material::FRONT_AND_BACK, Vec4(0.0, 0.1, 0.0, 1));
    segMaterial->setDiffuse(Material::FRONT_AND_BACK, Vec4(0.0, 0.6, 0.0, 1.0));
    segMaterial->setSpecular(Material::FRONT_AND_BACK, Vec4(0.0, 0.8, 0.0, 1));
    segMaterial->setShininess(Material::FRONT_AND_BACK, 64);

    Geometry* surface = new Geometry;

    surface->getOrCreateStateSet()->setAttributeAndModes(segMaterial.get(), StateAttribute::ON);

    Vec3Array* triangles = new Vec3Array;
    int triangle_count = 0;

    for (AdjacentVertex v: adjacentVertices) {
        if (v.right != nullptr) {
            Vec3 vbl(v.pos.x(), v.pos.y(), 1.0);
            Vec3 vbr(v.right->pos.x(), v.right->pos.y(), 1.0);
            Vec3 vtl(v.edgePos.x(), v.edgePos.y(), 1.0);
            Vec3 vtr(v.right->edgePos.x(), v.right->edgePos.y(), 1.0);

            triangles->push_back(vbl);
            triangles->push_back(vbr);
            triangles->push_back(vtl);
            triangles->push_back(vbr);
            triangles->push_back(vtl);
            triangles->push_back(vtr);

            triangle_count += 2;
        }
    }

    surface->setVertexArray(triangles);
    surface->addPrimitiveSet(new DrawArrays(PrimitiveSet::TRIANGLES, 0, triangle_count * 3));

    scene->addChild(surface);
}

void tester_seg(ref_ptr<Group> scene, GridHeightMap& heightMap, CrackSegment seg)
{
    CrackSegAABB segaabb(seg);

    for (int x = 0; x < heightMap.getWidth(); x++) {
        for (int y = 0; y < heightMap.getWidth(); y++) {
            if (segaabb.getSkipFace(x, y)) {
                heightMap.setShowFace(x, y, false);
            }
        }
    }

    std::vector<Vec2s>* encirclingVertices = segaabb.getEncirclingVertices();

    std::vector<AdjacentVertex> adjacentVertices;

    for (Vec2s p: *encirclingVertices) {
        adjacentVertices.push_back({ p, nullptr, nullptr, Vec2() });
    }

    //Sort by two predicates (length along the side and angle of vector from center)
    //[Sorts in order such that they form a loop around the segment)
    std::sort(adjacentVertices.begin(), adjacentVertices.end(), [seg] (AdjacentVertex a, AdjacentVertex b) {
        Vec2 apos(a.pos.x(), a.pos.y());
        Vec2 bpos(b.pos.x(), b.pos.y());

        float adist = seg.getRelativeAlongLine(apos);
        float bdist = seg.getRelativeAlongLine(bpos);

        if (adist < bdist) {
            return true;
        } else if (bdist < adist) {
            return false;
        } else {
            float aAngle = seg.getAngleFromCenter(apos);
            float bAngle = seg.getAngleFromCenter(bpos);

            return aAngle < bAngle;
        }
    });

    //Test for adjacency of vertices (most will be adjacent in scene if they are in the vector)
    for (int i = 0; i < adjacentVertices.size(); i++) {
        int nxt = i + 1;

        if (nxt >= adjacentVertices.size())
            nxt = 0;

        //Check if vertices are adjacent
        Vec2s cPos = adjacentVertices[i].pos;
        Vec2s nPos = adjacentVertices[nxt].pos;

        Vec2s diff = cPos - nPos;

        if (abs(diff.x()) <= 1 && abs(diff.y()) <= 1) {
            //Connect the vertices
            adjacentVertices[i].right = &adjacentVertices[nxt];
            adjacentVertices[nxt].left = &adjacentVertices[i];
        }
    }

    //Find edge positions
    for (AdjacentVertex &v: adjacentVertices) {
        v.edgePos = seg.getNearestPointOnEdge(Vec2(v.pos.x(), v.pos.y()));
    }

    //Find nearest points to each corner
    Vec2 segSide = seg.getSideways() * 0.5;
    Vec2 segEdgeVerts[] =  {
        seg.apos + segSide * seg.awidth, seg.apos - segSide * seg.awidth,
        seg.bpos + segSide * seg.bwidth, seg.bpos - segSide * seg.bwidth };

    for (Vec2 v: segEdgeVerts) {
        //Find closest adjacent vertex
        auto closest = std::min_element(adjacentVertices.begin(), adjacentVertices.end(),
                                        [v](AdjacentVertex a, AdjacentVertex b) {
            Vec2 apos(a.pos.x(), a.pos.y());
            Vec2 bpos(b.pos.x(), b.pos.y());
            return (v - apos).length2() < (v - bpos).length2();
        });

        closest->edgePos = v;
    }

    for (AdjacentVertex v: adjacentVertices) {
//        log_info << v.pos << " (" << seg.getRelativeAlongLine(Vec2(v.pos.x(), v.pos.y())) << " : " <<
//                    seg.getAngleFromCenter(Vec2(v.pos.x(), v.pos.y())) << ")" << std::endl;
        Vec2s lPos = v.left ? v.left->pos : Vec2s(-1, -1);
        Vec2s rPos = v.right ? v.right->pos : Vec2s(-1, -1);
        log_info << lPos << " <- " << v.pos << " -> " << rPos << " [" << v.edgePos << "]" << std::endl;
    }

    delete encirclingVertices;

    test_drawEdgeOfCrackSeg(scene, adjacentVertices);
}

void tester_seg_old1(GridHeightMap& heightMap, CrackSegment seg)
{
    std::vector<AdjacentVertex> adjacentVertices;

    //Find the vertices adjacent to the segment, and also set the ones
    //inside the segment
    for (int x = 0; x < heightMap.getWidth(); x++) {
        for (int y = 0; y < heightMap.getWidth(); y++) {
            if (seg.isInsideSeg(Vec2(x, y))) {
                heightMap.setSkip(x, y);
            } else {
                //Add to adjacent vertices if it's next to the seg
                bool adjacent = false;
                for (int offX = -1; offX < 2 && !adjacent; offX++) {
                    for (int offY = -1; offY < 2 && !adjacent; offY++) {
                        if (seg.isInsideSeg(Vec2(x + offX, y + offY)))
                            adjacent = true;
                    }
                }

                if (adjacent) {
                    adjacentVertices.push_back({ Vec2s(x, y), nullptr, nullptr });
                }
            }
        }
    }

    //Sort by two predicates (length along the side and angle of vector from center)
    //[Sorts in order such that they form a loop around the segment)
    std::sort(adjacentVertices.begin(), adjacentVertices.end(), [seg] (AdjacentVertex a, AdjacentVertex b) {
        Vec2 apos(a.pos.x(), a.pos.y());
        Vec2 bpos(b.pos.x(), b.pos.y());

        float adist = seg.getRelativeAlongLine(apos);
        float bdist = seg.getRelativeAlongLine(bpos);

        if (adist < bdist) {
            return true;
        } else if (bdist < adist) {
            return false;
        } else {
            float aAngle = seg.getAngleFromCenter(apos);
            float bAngle = seg.getAngleFromCenter(bpos);

            return aAngle < bAngle;
        }
    });

    //Test for adjacency of vertices (most will be adjacent in scene if they are in the vector)
    for (int i = 0; i < adjacentVertices.size(); i++) {
        int nxt = i + 1;

        if (nxt >= adjacentVertices.size())
            nxt = 0;

        //Check if vertices are adjacent
        Vec2s cPos = adjacentVertices[i].pos;
        Vec2s nPos = adjacentVertices[nxt].pos;

        Vec2s diff = cPos - nPos;

        if (abs(diff.x()) <= 1 && abs(diff.y()) <= 1) {
            //Connect the vertices
            adjacentVertices[i].right = &adjacentVertices[nxt];
            adjacentVertices[nxt].left = &adjacentVertices[i];
        }
    }

    for (AdjacentVertex v: adjacentVertices) {
//        log_info << v.pos << " (" << seg.getRelativeAlongLine(Vec2(v.pos.x(), v.pos.y())) << " : " <<
//                    seg.getAngleFromCenter(Vec2(v.pos.x(), v.pos.y())) << ")" << std::endl;
        Vec2s lPos = v.left ? v.left->pos : Vec2s(-1, -1);
        Vec2s rPos = v.right ? v.right->pos : Vec2s(-1, -1);
        log_info << lPos << " <- " << v.pos << " -> " << rPos << std::endl;
    }
}

void tester(ref_ptr<Group> scene)
{
    GridHeightMap heightMap(32, 31.0f);

    for (int x = 0; x < heightMap.getWidth(); x++) {
        for (int y = 0; y < heightMap.getWidth(); y++) {
            heightMap.set(x, y, 1.0f, false);
        }
    }

    CrackSegment seg = { Vec2(4, 8), 1, 5, Vec2(9, 3), 4, 5 };

    test_drawSeg(scene, seg);

    tester_seg(scene, heightMap, seg);

    StaticIceTile *tile = new StaticIceTile(32.0f, Vec2s(0,0));

    tile->setHeightMap(&heightMap);
    tile->show();

    scene->addChild(tile->getNode());
}

int main()
{
    LineSegment seg1(Vec2(0, 0), Vec2(0, 10));
    LineSegment seg2(Vec2(-5, 5), Vec2(5, 15.1));

    log_info << "Collide ? = " << seg1.intersects(seg2) << std::endl;
}

int main_old()
{
    osgViewer::Viewer viewer;
    ref_ptr<Group> scene(new Group);

    //Main light source
#ifdef TESTER_THINGY
    Vec3 lightPosition(0, 0, 10);
#else
    Vec3 lightPosition(000,000,4000);
#endif
    LightSource* ls = new LightSource;
    ls->getLight()->setPosition(Vec4(lightPosition,1));
    //ls->getLight()->setDirection(Vec3(0, -1.0f, 0));
    ls->getLight()->setAmbient(Vec4(0.5,0.5,0.5,1.0));
    ls->getLight()->setDiffuse(Vec4(0.6,0.6,0.6,1.0));
    ls->getLight()->setSpecular(Vec4(0.8, 0.8, 0.8, 1.0));
    ls->getLight()->setConstantAttenuation(0.35);

    scene->addChild(ls);

    // material
    ref_ptr<Material> material = new Material;
    material->setColorMode(Material::DIFFUSE);
    material->setAmbient(Material::FRONT_AND_BACK, Vec4(0.1, 0.1, 0.1, 1));
    material->setSpecular(Material::FRONT_AND_BACK, Vec4(1, 1, 1, 1));
    material->setShininess(Material::FRONT_AND_BACK, 64);
    scene->getOrCreateStateSet()->setAttributeAndModes(material.get(), StateAttribute::ON);

#ifdef TESTER_THINGY
    tester(scene);
#else
    //Tiled scene stuff
    ///int tile_count = 32;
    float tile_width = 500.0f * 0.4;
    Array2D<TerrainTile::TileType> types(50, 40);
    for (int x = 0; x < types.width(); x++) {
        for (int y = 0; y < types.height(); y++) {
            types.get(x, y) = TerrainTile::TYPE_STATIC_ICE;
        }
    }

    ref_ptr<TiledScene> tiledScene(new TiledScene(types, tile_width));
    tiledScene->setRenderDistance(10.0f);

    scene->addChild(tiledScene->getNode());
#endif

    viewer.setSceneData(scene.get());

    //Stats Event Handler s key
    viewer.addEventHandler(new osgViewer::StatsHandler);

    // add the state manipulator
    viewer.addEventHandler( new osgGA::StateSetManipulator(viewer.getCamera()->getOrCreateStateSet()) );

    //Windows size handler
    viewer.addEventHandler(new osgViewer::WindowSizeHandler);

    osgGA::TerrainManipulator* terrainMan = new osgGA::TerrainManipulator();
    viewer.setCameraManipulator(terrainMan);
    //terrainMan->setTransformation(Vec3(10.0, 10.0, 10.0), Vec3(0,0,0), Vec3(0, 1.0f, 0));

#ifndef TESTER_THINGY
    Vec3 center(13570.0 * 0.4, 13884.0 * 0.4, 30.0f);
    terrainMan->setHomePosition(center + Vec3(2000.0f, 2000.0f, 2000.0f), center, Vec3(0, 0, 1.0f));
#endif

    //return (viewer.run());

    ref_ptr<KeyboardController> keyboardCtrl(new KeyboardController);
    viewer.addEventHandler(keyboardCtrl);

    viewer.realize();

    while (!viewer.done()) {
        Vec3d cam_eye = Vec3d(0,0,0);
        Vec3d cam_center = Vec3d(0,0,0);
        Vec3d cam_up = Vec3d(0,0,0);
        terrainMan->getTransformation(cam_eye, cam_center, cam_up);

        Vec3d moveVec(0,0,0);
        if (keyboardCtrl->isKeyDown(osgGA::GUIEventAdapter::KEY_Up)) {
            moveVec += Vec3d(1.0, 0.0, 0.0);
        } else if (keyboardCtrl->isKeyDown(osgGA::GUIEventAdapter::KEY_Down)) {
            moveVec += Vec3d(-1.0, 0.0, 0.0);
        }
        if (keyboardCtrl->isKeyDown(osgGA::GUIEventAdapter::KEY_Left)) {
            moveVec += Vec3d(0.0, -1.0, 0.0);
        } else if (keyboardCtrl->isKeyDown(osgGA::GUIEventAdapter::KEY_Right)) {
            moveVec += Vec3d(0.0, 1.0, 0.0);
        }

        if (keyboardCtrl->isKeyDown(osgGA::GUIEventAdapter::KEY_K)) {
            log_info << "Pos: " << cam_center.x() << " " << cam_center.y() << " " << cam_center.z() << std::endl;
        }

        if (moveVec != Vec3d(0.0, 0.0, 0.0)) {
            double distToCenter = (cam_center - cam_eye).length();
            Vec3d moveDist = moveVec * distToCenter * 0.01;
            cam_center += moveDist;
            cam_eye += moveDist;

            terrainMan->setTransformation(cam_eye, cam_center, cam_up);
        }

#ifndef TESTER_THINGY
        tiledScene->updateCameraPosition(cam_center);
#endif

        viewer.frame();
    }

    return 0;
}

