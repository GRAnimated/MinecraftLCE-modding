#include "feature/BetaSpringFeature.h"

#include "Minecraft.Core/BlockPos.h"
#include "Minecraft.World/level/Level.h"
#include "Minecraft.World/level/block/Blocks.h"
#include "Minecraft.World/level/levelgen/feature/Feature.h"

BetaSpringFeature::BetaSpringFeature(Block* block) : Feature(false), mBlock(block) {}

bool BetaSpringFeature::place(Level* level, Random& random, const BlockPos& pos) {
    level->setInstaTick(true);

    if (level->getBlock(pos.above()) != Blocks::STONE)
        return false;
    if (level->getBlock(pos.below()) != Blocks::STONE)
        return false;
    if (level->getBlock(pos) != 0 && level->getBlock(pos) != Blocks::STONE)
        return false;
    int b1 = 0;
    if (level->getBlock(pos.west()) == Blocks::STONE)
        b1++;
    if (level->getBlock(pos.east()) == Blocks::STONE)
        b1++;
    if (level->getBlock(pos.north()) == Blocks::STONE)
        b1++;
    if (level->getBlock(pos.south()) == Blocks::STONE)
        b1++;
    int b2 = 0;
    if (level->isEmptyBlock(pos.west()))
        b2++;
    if (level->isEmptyBlock(pos.east()))
        b2++;
    if (level->isEmptyBlock(pos.north()))
        b2++;
    if (level->isEmptyBlock(pos.south()))
        b2++;
    if (b1 == 3 && b2 == 1) {
        const BlockState* state = mBlock->defaultBlockState();
        level->setBlock(pos, state, 2, false);
        // level->instaTick(pos, state, random);

        // mBlock->tick(level, pos, mBlock->defaultBlockState(), &random);
    }

    level->setInstaTick(false);

    return true;
}
