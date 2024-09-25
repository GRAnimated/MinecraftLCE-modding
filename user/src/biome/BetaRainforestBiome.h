#pragma once

#include "biome/BetaBiome.h"

class BetaRainforestBiome : public BetaBiome {
public:
    BetaRainforestBiome(Biome::EBiomeIDs id, Biome::BiomeProperties* properties);

    virtual Feature* getTreeFeature(Random&) override;
};
