#pragma once

#include "Minecraft.World/level/levelgen/feature/Feature.h"

class Block;

class BetaFlowerFeature : public Feature {
public:
    BetaFlowerFeature(Block* block);
    virtual ~BetaFlowerFeature();

    virtual bool place(Level* level, Random& random, const BlockPos& pos) override;

private:
    Block* mBlock;
};
