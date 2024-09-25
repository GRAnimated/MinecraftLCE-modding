#pragma once

#include "Minecraft.World/level/levelgen/feature/Feature.h"

class Block;

class BetaTreeFeature : public Feature {
public:
    BetaTreeFeature();

    virtual bool place(Level* level, Random& random, const BlockPos& pos) override;

private:
    bool canPlaceTree(Level* level, const BlockPos& pos, int height);
    void generateLeaves(Level* level, Random& random, const BlockPos& pos, int height);
    void generateTrunk(Level* level, const BlockPos& pos, int height);
};
