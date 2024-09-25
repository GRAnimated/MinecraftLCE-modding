#pragma once

#include "Minecraft.World/level/biome/BiomeDecorator.h"

class BetaBiomeDecorator : public BiomeDecorator {
public:
    BetaBiomeDecorator();

    virtual void decorate(Level*, Random&, Biome*, BlockPos const&) override;
    virtual void decorate(Biome*, Level*, Random&) override;
    virtual void decorateOres(Level*, Random&) override;

    static void initHooks();
};
