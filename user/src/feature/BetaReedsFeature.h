#pragma once

#include "Minecraft.World/level/levelgen/feature/Feature.h"

class Block;

class BetaReedsFeature : public Feature {
public:
    BetaReedsFeature();

    virtual bool place(Level* level, Random& random, const BlockPos& pos) override;
};
