#pragma once

#include "feature/BetaTreeFeature.h"

class Block;

class BetaBirchFeature : public BetaTreeFeature {
public:
    BetaBirchFeature();

    virtual bool place(Level* level, Random& random, const BlockPos& pos) override;

private:
    bool canPlaceTree(Level* level, const BlockPos& pos, int height);
    void generateLeaves(Level* level, Random& random, const BlockPos& pos, int height);
    void generateTrunk(Level* level, const BlockPos& pos, int height);
};
