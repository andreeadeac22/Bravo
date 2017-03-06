#include "AsyncTerrainUpdater.h"

#include "util/log.h"

void TerrainUpdater::process(UpdateJob job) {
    job.tile->setHeightMap(job.heightMap);
}
