#pragma once

#include "feature/BetaTreeFeature.h"

class Block;

class BetaPineFeature : public BetaTreeFeature {
public:
    BetaPineFeature();

    virtual bool place(Level* level, Random& random, const BlockPos& pos) override;
};
