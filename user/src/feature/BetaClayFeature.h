#pragma once

#include "Minecraft.World/level/levelgen/feature/Feature.h"

class Block;

class BetaClayFeature : public Feature {
public:
    BetaClayFeature(int count);

    virtual bool place(Level* level, Random& random, const BlockPos& pos) override;

private:
    int mCount;
};
