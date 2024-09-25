#include "biome/BetaTaigaBiome.h"
#include "Minecraft.World/Random.h"
#include "feature/BetaPineFeature.h"
#include "feature/BetaSpruceFeature.h"

BetaTaigaBiome::BetaTaigaBiome(Biome::EBiomeIDs id, Biome::BiomeProperties* properties)
    : BetaBiome(id, properties) {}

Feature* BetaTaigaBiome::getTreeFeature(Random& random) {
    return ((random.nextInt(3) == 0) ? (Feature*)new BetaPineFeature() : new BetaSpruceFeature());
}
