#ifndef ASYNCTERRAINUPDATER_H
#define ASYNCTERRAINUPDATER_H

#include <thread>
#include <vector>

#include "util/AsyncJobHandler.h"

#include "TerrainTile.h"

class AsyncTerrainUpdater : public AsyncJobHandler<TerrainTile*>
{
public:
    AsyncTerrainUpdater() {}

    virtual void process(TerrainTile* tile);

};

#endif // ASYNCTERRAINUPDATER_H
