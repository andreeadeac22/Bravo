
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

#include <iostream>

using namespace osg;

int main()
{
    osgViewer::Viewer viewer;
    ref_ptr<Group> scene(new Group);

    //Main light source
    Vec3 lightPosition(500,500,4000);
    LightSource* ls = new LightSource;
    ls->getLight()->setPosition(Vec4(lightPosition,1));
    //ls->getLight()->setDirection(Vec3(0, -1.0f, 0));
    ls->getLight()->setAmbient(Vec4(0.5,0.5,0.5,1.0));
    ls->getLight()->setDiffuse(Vec4(0.6,0.6,0.6,1.0));
    ls->getLight()->setSpecular(Vec4(0.8, 0.8, 0.8, 1.0));
    ls->getLight()->setConstantAttenuation(0.4);

    scene->addChild(ls);

    // material
    ref_ptr<Material> material = new Material;
    material->setColorMode(Material::DIFFUSE);
    material->setAmbient(Material::FRONT_AND_BACK, Vec4(0.1, 0.1, 0.1, 1));
    material->setSpecular(Material::FRONT_AND_BACK, Vec4(1, 1, 1, 1));
    material->setShininess(Material::FRONT_AND_BACK, 64);
    scene->getOrCreateStateSet()->setAttributeAndModes(material.get(), StateAttribute::ON);

    //Tiled scene stuff
    float tile_width = 128.0f;
    Array2D<TerrainTile::TileType> types(34, 34);
    for (int x = 0; x < types.width(); x++) {
        for (int y = 0; y < types.height(); y++) {
            types.get(x, y) = TerrainTile::TYPE_STATIC_BOUNDARY_ICE;
        }
    }

    ref_ptr<TiledScene> tiledScene(new TiledScene(types, tile_width));
    tiledScene->setRenderDistance(6.0f);

    scene->addChild(tiledScene->getNode());

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

    Vec3 t_center(2895.55, 2837, 26.7789);
    Vec3 t_eye(2584.64, 2526.09, 337.695);
    //Vec3 t_center(0,0,0);
    //Vec3 t_eye(-500, -500, 500);
    terrainMan->setHomePosition(t_eye, t_center, Vec3(0, 0, 1.0f));

    //return (viewer.run());

    ref_ptr<KeyboardController> keyboardCtrl(new KeyboardController);
    viewer.addEventHandler(keyboardCtrl);

    viewer.realize();

    int cameraUpdateFrame = 20;

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
        } else if (keyboardCtrl->isKeyDown(osgGA::GUIEventAdapter::KEY_Left)) {
            moveVec += Vec3d(0.0, -1.0, 0.0);
        } else if (keyboardCtrl->isKeyDown(osgGA::GUIEventAdapter::KEY_Right)) {
            moveVec += Vec3d(0.0, 1.0, 0.0);
        }

        if (keyboardCtrl->isKeyDown(osgGA::GUIEventAdapter::KEY_K)) {
            log_info << "Pos: " << cam_center << std::endl;
            log_info << "Eye: " << cam_eye << std::endl;
        }

        if (moveVec != Vec3d(0.0, 0.0, 0.0)) {
            double distToCenter = (cam_center - cam_eye).length();
            Vec3d moveDist = moveVec * (distToCenter * 0.05);
            cam_center += moveDist;
            cam_eye += moveDist;

            terrainMan->setTransformation(cam_eye, cam_center, cam_up);
        }

        if (--cameraUpdateFrame <= 0) {
            tiledScene->updateCameraPosition(cam_center);
            cameraUpdateFrame = 20;
        }

        viewer.frame();
    }

    return 0;
}

