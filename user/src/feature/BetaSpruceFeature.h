#pragma once

#include "feature/BetaTreeFeature.h"

class Block;

class BetaSpruceFeature : public BetaTreeFeature {
public:
    BetaSpruceFeature();

    virtual bool place(Level* level, Random& random, const BlockPos& pos) override;
};
