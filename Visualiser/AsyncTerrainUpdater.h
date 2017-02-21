#ifndef ASYNCTERRAINUPDATER_H
#define ASYNCTERRAINUPDATER_H

#include <thread>

#include "util/AtomicQueue.h"

#include "TerrainTile.h"

class AsyncTerrainUpdater
{
public:
    AsyncTerrainUpdater();

    /**
     * @brief Queue an update for this terrain tile
     * @param tile
     */
    void enqueueUpdate(TerrainTile* tile) {
        updateQueue.push(tile);
    }

    void run_thread();

    void stop() {
        updateQueue.notifyAll();
        thread.join();
    }

private:
    AtomicQueue<TerrainTile*> updateQueue;

    std::thread thread;

};

#endif // ASYNCTERRAINUPDATER_H
