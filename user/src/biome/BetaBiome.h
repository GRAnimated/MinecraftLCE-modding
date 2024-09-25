#pragma once

#include "Minecraft.World/level/biome/Biome.h"

class BetaBiome : public Biome {
public:
    static Biome* map[4096];
    static void recalc();
    static Biome* getBiome(double paramDouble1, double paramDouble2);
    static Biome* _getBiome(float paramFloat1, float paramFloat2);

    BetaBiome(Biome::EBiomeIDs id, Biome::BiomeProperties* properties);

    virtual int getBaseClass() override;

    virtual uint getGrassColor(BlockPos const*) override;
    virtual uint getFoliageColor(BlockPos const*) override;
    virtual void setGrassColor(BlockPos const*, uint) override;
    virtual void setFoliageColor(BlockPos const*, uint) override;

    virtual void decorate(Level* level, Random& random, const BlockPos* pos) override;

    static void initHooks();
};
