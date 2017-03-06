#include "AsyncTerrainLoader.h"

#include <algorithm>

#include "../Datastore/TileStore.h"

#include "TiledScene.h"

using namespace osg;

AsyncTerrainLoader::AsyncTerrainLoader(osg::Vec2i tilingDim, TiledScene& tsc)
    : scene(tsc), center(0, 0), centerChanged(true), heightMaps(tilingDim.x(), tilingDim.y()), running(true)
{
    //Set all the tiles to load (all of them)
    tilesToLoad.reserve(tilingDim.x()*tilingDim.y());
    for (int x = 0; x < tilingDim.x(); x++) {
        for (int y = 0; y < tilingDim.y(); y++) {
            tilesToLoad.push_back(Vec2i(x, y));
        }
    }

    heightMaps.init(nullptr);

    loaderThread = new std::thread(&AsyncTerrainLoader::run_thread, this);
}

AsyncTerrainLoader::~AsyncTerrainLoader()
{
    delete loaderThread;

    heightMaps.forEach([](int x, int y, HeightMap* val) {
        if (val != nullptr)
            delete val;
    });
}

void AsyncTerrainLoader::setCurrentCenter(osg::Vec2i c)
{
    std::unique_lock<std::mutex> lk(loaderMutex);

    center = c;
    centerChanged = true;
}

void AsyncTerrainLoader::run_thread()
{
    //TODO: Get all constants from elsewhere
    float tile_width = 128.0f;

    TileStore tl("bravodb");

    while (running && remainingTiles() > 0) {
        updateCenter();

        Vec2i nxtTile = getNextHeightMapJob();

        SquareTile * t = tl.getTileAt(nxtTile.x(), nxtTile.y());

        int width = 128;
        HeightMap* heightMap = new HeightMap(width, tile_width);

        if (t != nullptr) {
            //Copy data from t to heightMap
            for (int x = 0; x < width; x++) {
                for (int y = 0; y < width; y++) {
                    float height = t->getHeightAt(x, y) * 0.1f;
                    heightMap->set(x, y, height, height == 0.0f);
                }
            }
            delete t;
        }

        heightMaps.get(nxtTile.x(), nxtTile.y()) = heightMap;

        updateToLeft(nxtTile.x(), nxtTile.y());
        updateToRight(nxtTile.x(), nxtTile.y());
        updateToBottom(nxtTile.x(), nxtTile.y());
        updateToTop(nxtTile.x(), nxtTile.y());
        updateToBottomLeft(nxtTile.x(), nxtTile.y());
        updateToBottomRight(nxtTile.x(), nxtTile.y());
        updateToTopLeft(nxtTile.x(), nxtTile.y());
        updateToTopRight(nxtTile.x(), nxtTile.y());
    }
}

/** Find the next height map to load based on distance **/
osg::Vec2i AsyncTerrainLoader::getNextHeightMapJob()
{
    Vec2i nxt = tilesToLoad.back();
    tilesToLoad.pop_back();
    return nxt;
}

void AsyncTerrainLoader::updateCenter()
{
    std::unique_lock<std::mutex> lk(loaderMutex);

    if (centerChanged) {
        centerChanged = false;
        lk.unlock();

        //Change the order of tile loading (descending order so back can be used)
        std::sort(tilesToLoad.begin(), tilesToLoad.end(), [this](const Vec2i& a, const Vec2i& b) {
            Vec2i c2a = a - this->center;
            Vec2i c2b = b - this->center;
            Vec2f c2af(c2a.x(), c2a.y());
            Vec2f c2bf(c2b.x(), c2b.y());
            return c2af.length2() > c2bf.length2();
            //return (c2a.x()*c2a.x() + c2a.y()*c2a.y()) < (c2b.x()*c2b.x() + c2b.y()*c2b.y());
        });
    }
}

int AsyncTerrainLoader::remainingTiles()
{
    return tilesToLoad.size();
}

void AsyncTerrainLoader::pushHeightMapUpdates(int x, int y)
{
    bool left = upToDate(x-1, y);
    bool right = upToDate(x+1, y);
    bool bottom = upToDate(x, y-1);
    bool top = upToDate(x, y+1);
    bool botleft = upToDate(x-1, y-1);
    bool botright = upToDate(x+1, y-1);
    bool topleft = upToDate(x-1, y+1);
    bool topright = upToDate(x+1, y+1);

    if (left && right && bottom && top &&
            botleft && botright && topleft && topright) {
        HeightMap* heightMap = heightMaps.get(x, y);
        heightMap->calcBoundaries();
        scene.updateTileAt(Vec2i(x, y), heightMap);
    }
}
