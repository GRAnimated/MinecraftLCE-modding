#pragma once

#include "biome/BetaBiome.h"

class BetaForestBiome : public BetaBiome {
public:
    BetaForestBiome(Biome::EBiomeIDs id, Biome::BiomeProperties* properties);

    virtual Feature* getTreeFeature(Random&) override;
};
