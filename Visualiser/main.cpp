
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

#include <glm/gtc/noise.hpp>
#include <glm/gtc/random.hpp>

#include "util/Array2d.h"

#include "TiledScene.h"
#include "HeightMap.h"

#include <iostream>

using namespace osg;

Array2D<float>* genHeightMap(int width, int height)
{
    Array2D<float>* heightMap = new Array2D<float>(width, height);

    float noiseSamples[] = { 0.1f, 0.01 };
    float sampleHeights[] = { 3.0f, 20.0f };
    int numSamples = 2;

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            float sample = 0.0f;

            for (int i = 0; i < numSamples; i++) {
                float unitSpacing = noiseSamples[i];
                glm::vec2 noiseCoord(unitSpacing * x, unitSpacing * y);
                sample += glm::simplex(noiseCoord) * sampleHeights[i];
            }

            heightMap->get(x, y) = sample;
        }
    }

    return heightMap;
}

Geometry* genSceneFromHeightMap(Array2D<float> *heightMap)
{
    Geometry* surface = new Geometry;

    Vec3Array* colors = new Vec3Array;
    colors->push_back(Vec3(1.0f, 1.0f, 1.0f));

    Vec3Array* triangles = new Vec3Array;

    Vec2Array* texcoords = new Vec2Array;

    float spc = 4.0f;
    float offsetX = heightMap->width() / -2.0f * spc;
    float offsetY = heightMap->height() / -2.0f * spc;

    int triangle_count = 0;

    for (int x = 0; x < heightMap->width() - 1; x++) {
        for (int z = 0; z < heightMap->height() - 1; z++) {
            float xf = offsetX + (float)x * spc;
            float zf = offsetY + (float)z * spc;

            //Lower triangle
            triangles->push_back(Vec3(xf, zf, heightMap->get(x, z)));
            triangles->push_back(Vec3(xf + spc, zf, heightMap->get(x + 1, z)));
            triangles->push_back(Vec3(xf, zf + spc, heightMap->get(x, z + 1)));

            //Upper
            triangles->push_back(Vec3(xf + spc, zf, heightMap->get(x + 1, z)));
            triangles->push_back(Vec3(xf + spc, zf + spc, heightMap->get(x + 1, z + 1)));
            triangles->push_back(Vec3(xf, zf + spc, heightMap->get(x, z + 1)));

            //Tex coords
            texcoords->push_back(Vec2(0.2f, 0.2f));
            texcoords->push_back(Vec2(0.8f, 0.2f));
            texcoords->push_back(Vec2(0.0f, 0.8f));
            texcoords->push_back(Vec2(0.8f, 0.2f));
            texcoords->push_back(Vec2(0.8f, 0.8f));
            texcoords->push_back(Vec2(0.2f, 0.8f));

            triangle_count++;
        }
    }

    surface->setVertexArray(triangles);
    surface->setColorArray(colors);
    //surface->setTexCoordArray(0, texcoords);
    surface->addPrimitiveSet(new DrawArrays(PrimitiveSet::TRIANGLES, 0, triangle_count * 6));

    return surface;
}

void genTileTypeMap(Array2D<TerrainTile::TileType> &types, float tileWidth)
{
    //Generate types at vertices
    Array2D<TerrainTile::TileType> vertexTypes(types.width() + 1, types.height() + 1);

    for (int x = 0; x < vertexTypes.width(); x++) {
        for (int y = 0; y < vertexTypes.height(); y++) {
            glm::vec2 noiseCoord(tileWidth * x, tileWidth * y);
            vertexTypes.get(x, y) = glm::simplex(noiseCoord) > 0.3 ? TerrainTile::TYPE_STATIC_ICE : TerrainTile::TYPE_WATER;
        }
    }

    //Figure out type of each point
    for (int x = 0; x < types.width(); x++) {
        for (int y = 0; y < types.height(); y++) {
            //If all corners are same, thats the type, if they're different, theres a boundary
            if (vertexTypes.get(x, y) == vertexTypes.get(x + 1, y) &&
                    vertexTypes.get(x + 1, y) == vertexTypes.get(x + 1, y + 1) &&
                    vertexTypes.get(x + 1, y + 1) == vertexTypes.get(x, y + 1)) {
                types.get(x, y) = vertexTypes.get(x, y);
            } else {
                types.get(x, y) = TerrainTile::TYPE_STATIC_BOUNDARY_ICE;
            }
        }
    }
}

void generateHeightMap(GridHeightMap &heightMap, int tile_x, int tile_y, float tile_width)
{
    int tileVertexCount = heightMap.getWidth();

    float largeSimplexTileWidth = tile_width * 0.001f;
    float smallSimplexTileWidth = tile_width * 0.05f;

    //Large simplex vertex separation
    float lrgSpxVertSep = largeSimplexTileWidth / (tileVertexCount - 1);

    float lrgSpxOffsetX = tile_x * largeSimplexTileWidth;
    float lrgSpxOffsetY = tile_y * largeSimplexTileWidth;

    float smlSpxVertSep = smallSimplexTileWidth / (tileVertexCount - 1);

    float smlSpxOffsetX = tile_x * smallSimplexTileWidth;
    float smlSpxOffsetY = tile_y * smallSimplexTileWidth;

    for (int x = -1; x < tileVertexCount + 1; x++) {
        for (int y = -1; y < tileVertexCount + 1; y++) {
            glm::vec2 lrgSpxCoord(lrgSpxOffsetX + x * lrgSpxVertSep,
                                  lrgSpxOffsetY + y * lrgSpxVertSep);

            //Determine whether the point is water or ice and the height at this point
            if (glm::simplex(lrgSpxCoord) > 0.3) {
                glm::vec2 smlSpxCoord(smlSpxOffsetX + x * smlSpxVertSep,
                                      smlSpxOffsetY + y * smlSpxVertSep);

                heightMap.set(x, y, 20.0f + glm::simplex(smlSpxCoord) * 1.0f, false);
            } else {
                heightMap.set(x, y, 0.0f, true);
            }

        }
    }

    heightMap.calcBoundaries();
}



int main()
{
    osgViewer::Viewer viewer;
    ref_ptr<Group> scene(new Group);

    //Main light source
    Vec3 lightPosition(800,800,800);
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
    int tile_count = 16;
    float tile_width = 80.0f;
    Array2D<TerrainTile::TileType> types(tile_count, tile_count);
    genTileTypeMap(types, tile_width * 0.001f);

    ref_ptr<TiledScene> tiledScene(new TiledScene(types, tile_width));

    for (int x = 0; x < types.width(); x++) {
        for (int y = 0; y < types.height(); y++) {
            TerrainTile::TileType tileType = types.get(x, y);

            if (tileType == TerrainTile::TYPE_STATIC_ICE) {
                GridHeightMap heightMap(16, tile_width);
                generateHeightMap(heightMap, x, y, tile_width);
                tiledScene->setHeightMap(x, y, &heightMap);
            } else if (tileType == TerrainTile::TYPE_STATIC_BOUNDARY_ICE) {
                GridHeightMap heightMap(16, tile_width);
                generateHeightMap(heightMap, x, y, tile_width);
                tiledScene->setHeightMap(x, y, &heightMap);
            }
        }
    }

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
    terrainMan->setTransformation(Vec3(10.0, 10.0, 10.0), Vec3(0,0,0), Vec3(0, 1.0f, 0));

    return (viewer.run());
}

