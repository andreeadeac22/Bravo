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
#include <osgGA/FlightManipulator>

 #include <QMediaPlayer>

#include "util/Array2d.h"
#include "util/log.h"

#include "TiledScene.h"
#include "HeightMap.h"
#include "KeyboardController.h"

#include <iostream>

using namespace osg;

int main(int argc, char** argv)
{
    if (argc != 5) {
        std::cout << "Usage: ./" << argv[0] << " <db name> <tile width> <tiles in x> <tiles in y>" << std::endl;
        std::cout << "(Where tiles in x/y is the number of tiles to load from the database in each dimension)" << std::endl;
        return 1;
    }

    int tile_vert_width = 1;
    int tile_count_x = 1;
    int tile_count_y = 1;

    std::string databaseName(argv[1]);

    try {
        tile_vert_width = std::stoi(argv[2]);
        tile_count_x = std::stoi(argv[3]);
        tile_count_y = std::stoi(argv[4]);
    } catch (std::exception e) {
        std::cerr << "Failed to parse arguments." << std::endl;
        return 1;
    }

    osgViewer::Viewer viewer;
    ref_ptr<Group> scene(new Group);

    //Main light source
    Vec3 lightPosition(500,500,4000);
    LightSource* ls = new LightSource;
    ls->getLight()->setPosition(Vec4(lightPosition,1));
    ls->getLight()->setAmbient(Vec4(0.5,0.5,0.5,1.0));
    ls->getLight()->setDiffuse(Vec4(0.6,0.6,0.6,1.0));
    ls->getLight()->setSpecular(Vec4(0.8, 0.8, 0.8, 1.0));
    ls->getLight()->setConstantAttenuation(0.4);

    scene->addChild(ls);

    //Tiled scene 
    Array2D<TerrainTile::TileType> types(tile_count_x, tile_count_y);
    for (int x = 0; x < types.width(); x++) {
        for (int y = 0; y < types.height(); y++) {
            types.get(x, y) = TerrainTile::TYPE_STATIC_ICE;
        }
    }

    ref_ptr<TiledScene> tiledScene(new TiledScene(types, tile_vert_width, databaseName));

    scene->addChild(tiledScene->getNode());

    viewer.setSceneData(scene.get());

    //Stats Event Handler's key
    viewer.addEventHandler(new osgViewer::StatsHandler);

    //Add the state manipulator
    viewer.addEventHandler( new osgGA::StateSetManipulator(viewer.getCamera()->getOrCreateStateSet()) );

    //Windows size handler
    viewer.addEventHandler(new osgViewer::WindowSizeHandler);

    osgGA::TerrainManipulator* terrainMan = new osgGA::TerrainManipulator();
    viewer.setCameraManipulator(terrainMan);

    Vec3 t_center(0,0,0);
    Vec3 t_eye(-500, -500, 500);
    terrainMan->setHomePosition(t_eye, t_center, Vec3(0, 0, 1.0f));

    ref_ptr<KeyboardController> keyboardCtrl(new KeyboardController);
    viewer.addEventHandler(keyboardCtrl);

    //set up windows and associated threads
    viewer.realize();
    Vec3d cam_eye;
    Vec3d cam_center;
    Vec3d cam_up;

    int cameraUpdateFrame = 20;

    //play background music
    QMediaPlayer *music = new QMediaPlayer();
    music->setMedia(QUrl("qrc:/sounds/wind.wav"));
    music->play();

    while (!viewer.done()) {
        terrainMan->getTransformation(cam_eye, cam_center, cam_up);

        Vec3d moveVec(0,0,0);
        if (keyboardCtrl->isKeyDown(osgGA::GUIEventAdapter::KEY_Up)) {
            moveVec += Vec3d(1, 0, 0);
        } else if (keyboardCtrl->isKeyDown(osgGA::GUIEventAdapter::KEY_Down)) {
            moveVec += Vec3d(-1, 0, 0);
        } else if (keyboardCtrl->isKeyDown(osgGA::GUIEventAdapter::KEY_Left)) {
            moveVec += Vec3d(0, 1, 0);
        } else if (keyboardCtrl->isKeyDown(osgGA::GUIEventAdapter::KEY_Right)) {
            moveVec += Vec3d(0, -1, 0);
        } 

        if (moveVec != Vec3d(0.0, 0.0, 0.0)) {
            double distToCenter = (cam_center - cam_eye).length();
            Vec3d moveDist = moveVec * (distToCenter * 0.01);
            cam_center += moveDist;
            cam_eye += moveDist;

            terrainMan->setTransformation(cam_eye, cam_center, cam_up);
        }

        if (--cameraUpdateFrame <= 0) {
            tiledScene->updateCameraPosition(cam_eye);
            cameraUpdateFrame = 5;
        }

        viewer.frame();
    }

    return 0;
}

