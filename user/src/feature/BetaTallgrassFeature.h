#pragma once

#include "Minecraft.World/level/levelgen/feature/Feature.h"

class Block;

class BetaTallgrassFeature : public Feature {
public:
    BetaTallgrassFeature(const BlockState* block);
    virtual ~BetaTallgrassFeature();

    virtual bool place(Level* level, Random& random, const BlockPos& pos) override;

private:
    const BlockState* mBlockState;
};
