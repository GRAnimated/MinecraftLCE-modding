#include "biome/BetaRainforestBiome.h"
#include "Minecraft.World/Random.h"
#include "feature/BetaBasicTree.h"
#include "feature/BetaTreeFeature.h"

BetaRainforestBiome::BetaRainforestBiome(Biome::EBiomeIDs id, Biome::BiomeProperties* properties)
    : BetaBiome(id, properties) {}

Feature* BetaRainforestBiome::getTreeFeature(Random& random) {
    return ((random.nextInt(3) == 0) ? (Feature*)new BetaBasicTree() : new BetaTreeFeature());
}
