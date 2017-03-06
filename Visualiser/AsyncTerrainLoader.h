#pragma once

#include <thread>
#include <mutex>
#include <vector>

#include <osg/Vec2i>

#include "util/Array2d.h"

#include "HeightMap.h"

//Forward declaration to avoid header issues
class TiledScene;

/**
 * @brief Asynchronously loads tiles
 * from the database and updates the height maps locally.
 * An instantiation of this is designed to operate as the sole provider of
 * height map data. The TiledScene class keeps the "center" up to date
 * (facilitating deciding the next tiles to load).
 * On loading a tile, it will use this tile to update neighbouring tiles, when
 * a tile contains it's own data and data for adjacent tiles it is ready to be
 * used to generate a vertex buffer, and is sent back to TiledScene to be
 * queued as an update.
 */
class AsyncTerrainLoader
{
public:
    /**
     * @brief Construct a Asynchronous terrain loader
     * @param tilingDim Dimensions (in terms of width and height) of the tile space
     * E.g. (4, 5) means tiles in X span from 0->3 and in Y span from 0->4
     * @param tsc   Reference to the tiled scene object
     * @param tileVWidth    Number of vertices per tile
     * @param db    Name of database to connect to
     */
    AsyncTerrainLoader(osg::Vec2i tilingDim, TiledScene& tsc, int tileVWidth, std::string db);

    ~AsyncTerrainLoader();

    /**
     * @brief Set the centre of the players view
     * @param c
     */
    void setCurrentCenter(osg::Vec2i c);

    /** Ask the thread to stop */
    void stop() {
        running = false;
    }

private:
    TiledScene& scene;

    std::thread *loaderThread;

    osg::Vec2i center;
    bool centerChanged;

    int tileVertWidth;

    std::string dbname;

    Array2D<HeightMap*> heightMaps;

    //Used to find the next nearest tile to load
    std::vector<osg::Vec2i> tilesToLoad;

    std::mutex loaderMutex;

    bool running;

    void run_thread();

    /**
     * @brief Find the next height map to load based on distance
     * @return Next tile location to update
     */
    osg::Vec2i getNextHeightMapJob();

    void updateCenter();

    /**
     * @brief remainingTiles
     * @return Number of remaining tiles
     */
    int remainingTiles();

    /**
     * @brief Check if this coordinate is within the range of the height maps
     */
    bool rangeCheck(int x, int y) {
        return x >= 0 && x < heightMaps.width() && y >= 0 && y < heightMaps.height();
    }

    /**
     * @brief Check whether a tile is loaded
     */
    bool isTileLoaded(int x, int y) {
        return rangeCheck(x, y) && heightMaps.get(x, y) != nullptr;
    }

    /** Check if a tile at this point has updated it's surrounding tiles */
    bool upToDate(int x, int y) {
        return !rangeCheck(x, y) || heightMaps.get(x, y) != nullptr;
    }

    /**
     * @brief Check if a height map at this position is loaded properly
     * (i.e. adjacent tiles have updated it) and if it has, send this
     * map to be rendered
     * @param x
     * @param y
     */
    void pushHeightMapUpdates(int x, int y);

    /** Update the tile to the left with this tile */
    void updateToLeft(int x, int y) {
        if (isTileLoaded(x, y) && isTileLoaded(x-1, y)) {
            HeightMap* a = heightMaps.get(x, y);
            HeightMap* b = heightMaps.get(x-1, y);
            pushHeightMapUpdates(x-1, y);
            a->copyFromLeft(b);
            b->copyFromRight(a);
        }
    }

    /** Update the tile to the right with this tile */
    void updateToRight(int x, int y) {
        if (isTileLoaded(x, y) && isTileLoaded(x+1, y)) {
            HeightMap* a = heightMaps.get(x, y);
            HeightMap* b = heightMaps.get(x+1, y);
            pushHeightMapUpdates(x+1, y);
            a->copyFromRight(b);
            b->copyFromLeft(a);
        }
    }

    /** Update the tile to the bottom with this tile */
    void updateToBottom(int x, int y) {
        if (isTileLoaded(x, y) && isTileLoaded(x, y-1)) {
            HeightMap* a = heightMaps.get(x, y);
            HeightMap* b = heightMaps.get(x, y-1);
            pushHeightMapUpdates(x, y-1);
            a->copyFromBottom(b);
            b->copyFromTop(a);
        }
    }

    /** Update the tile to the top with this tile */
    void updateToTop(int x, int y) {
        if (isTileLoaded(x, y) && isTileLoaded(x, y+1)) {
            HeightMap* a = heightMaps.get(x, y);
            HeightMap* b = heightMaps.get(x, y+1);
            pushHeightMapUpdates(x, y+1);
            a->copyFromTop(b);
            b->copyFromBottom(a);
        }
    }

    /** Update the tile to the bottom left with this tile */
    void updateToBottomLeft(int x, int y) {
        if (isTileLoaded(x, y) && isTileLoaded(x-1, y-1)) {
            HeightMap* a = heightMaps.get(x, y);
            HeightMap* b = heightMaps.get(x-1, y-1);
            pushHeightMapUpdates(x-1, y-1);
            a->copyFromBottomLeft(b);
            b->copyFromTopRight(a);
        }
    }

    /** Update the tile to the bottom right with this tile */
    void updateToBottomRight(int x, int y) {
        if (isTileLoaded(x, y) && isTileLoaded(x+1, y-1)) {
            HeightMap* a = heightMaps.get(x, y);
            HeightMap* b = heightMaps.get(x+1, y-1);
            pushHeightMapUpdates(x+1, y-1);
            a->copyFromBottomRight(b);
            b->copyFromTopLeft(a);
        }
    }

    /** Update the tile to the top left with this tile */
    void updateToTopLeft(int x, int y) {
        if (isTileLoaded(x, y) && isTileLoaded(x-1, y+1)) {
            HeightMap* a = heightMaps.get(x, y);
            HeightMap* b = heightMaps.get(x-1, y+1);
            pushHeightMapUpdates(x-1, y+1);
            a->copyFromTopLeft(b);
            b->copyFromBottomRight(a);
        }
    }

    /** Update the tile to the top right with this tile */
    void updateToTopRight(int x, int y) {
        if (isTileLoaded(x, y) && isTileLoaded(x+1, y+1)) {
            HeightMap* a = heightMaps.get(x, y);
            HeightMap* b = heightMaps.get(x+1, y+1);
            pushHeightMapUpdates(x+1, y+1);
            a->copyFromTopRight(b);
            b->copyFromBottomLeft(a);
        }
    }

};
