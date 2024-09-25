#include "feature/BetaTallgrassFeature.h"
#include "Minecraft.Core/BlockPos.h"
#include "Minecraft.World/Random.h"
#include "Minecraft.World/level/Level.h"
#include "Minecraft.World/level/block/Block.h"
#include "Minecraft.World/level/block/Blocks.h"
#include "Minecraft.World/level/block/state/BlockState.h"
#include "Minecraft.World/level/material/Material.h"

BetaTallgrassFeature::BetaTallgrassFeature(const BlockState* state)
    : Feature(false), mBlockState(state) {}

BetaTallgrassFeature::~BetaTallgrassFeature() {}

bool BetaTallgrassFeature::place(Level* level, Random& random, const BlockPos& pos) {
    BlockPos currentPos = pos;
    const BlockState* blockState = level->getBlockState(currentPos);
    while ((blockState->getMaterial() == (Material*)Material::AIR ||
            blockState->getMaterial() == (Material*)Material::LEAVES) &&
           currentPos.getY() > 0) {
        currentPos = currentPos.below();
        blockState = level->getBlockState(currentPos);
    }

    for (int i = 128; i > 0; --i) {
        int n1 = random.nextInt(8);
        int n2 = n1 - random.nextInt(8);
        int n3 = random.nextInt(4);
        int v13 = n3 - random.nextInt(4);
        int v14 = random.nextInt(8);
        int v15 = random.nextInt(8);
        BlockPos v18 = pos.offset(n2, v13, v14 - v15);

        if (level->isEmptyBlock(v18) && level->getBlock(v18.below()) == Blocks::GRASS) {
            level->setBlock(v18, mBlockState, 2, false);
        }
    }

    return true;
}
