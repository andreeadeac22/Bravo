#ifndef ASYNCTERRAINUPDATER_H
#define ASYNCTERRAINUPDATER_H

#include <thread>
#include <vector>

#include "util/AtomicQueue.h"

#include "TerrainTile.h"

class AsyncTerrainUpdater
{
public:
    AsyncTerrainUpdater(int num_threads = 1);

    ~AsyncTerrainUpdater();

    /**
     * @brief Stop all threads
     */
    void stop();

    /**
     * @brief Queue an update for this terrain tile
     * @param tile
     */
    void enqueueUpdate(TerrainTile* tile) {
        updateQueue.push(tile);
    }

    void run_thread();
private:
    AtomicQueue<TerrainTile*> updateQueue;

    std::vector<std::thread*> threads;

};

#endif // ASYNCTERRAINUPDATER_H
