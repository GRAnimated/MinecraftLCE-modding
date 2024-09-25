#pragma once

#include "biome/BetaBiome.h"

class BetaTaigaBiome : public BetaBiome {
public:
    BetaTaigaBiome(Biome::EBiomeIDs id, Biome::BiomeProperties* properties);

    virtual Feature* getTreeFeature(Random&) override;
};
