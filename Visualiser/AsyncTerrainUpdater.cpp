#include "AsyncTerrainUpdater.h"

#include "util/log.h"
#include "../Datastore/TileStore.h"

#include <iostream>

void TerrainUpdater::process(UpdateJob job) {
    job.tile->setHeightMap(job.heightMap);
}
