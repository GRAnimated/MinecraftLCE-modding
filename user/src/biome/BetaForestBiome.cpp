#include "biome/BetaForestBiome.h"
#include "Minecraft.World/Random.h"
#include "feature/BetaBasicTree.h"
#include "feature/BetaBirchFeature.h"
#include "feature/BetaTreeFeature.h"

BetaForestBiome::BetaForestBiome(Biome::EBiomeIDs id, Biome::BiomeProperties* properties)
    : BetaBiome(id, properties) {}

Feature* BetaForestBiome::getTreeFeature(Random& random) {
    return ((random.nextInt(5) == 0) ?
                (Feature*)new BetaBirchFeature() :
                ((random.nextInt(3) == 0) ? new BetaBasicTree() : new BetaTreeFeature()));
}
