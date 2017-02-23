#pragma once

#include <thread>
#include <vector>

#include "util/AsyncJobHandler.h"

#include "TerrainTile.h"

/**
 * @brief Class to manage asynchronous terrain loading
 */
class AsyncTerrainUpdater : public AsyncJobHandler<TerrainTile*>
{
public:
    AsyncTerrainUpdater() {}

    virtual void process(TerrainTile* tile);

};
