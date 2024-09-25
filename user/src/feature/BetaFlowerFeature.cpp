#include "feature/BetaFlowerFeature.h"
#include "Minecraft.Core/BlockPos.h"
#include "Minecraft.World/Random.h"
#include "Minecraft.World/level/Level.h"
#include "Minecraft.World/level/block/Block.h"
#include "Minecraft.World/level/block/Blocks.h"

BetaFlowerFeature::BetaFlowerFeature(Block* block) : Feature(false), mBlock(block) {}

BetaFlowerFeature::~BetaFlowerFeature() {}

bool BetaFlowerFeature::place(Level* level, Random& random, const BlockPos& pos) {
    for (int i = 0; i < 64; ++i) {
        int x = pos.x + random.nextInt(8) - random.nextInt(8);
        int y = pos.y + random.nextInt(4) - random.nextInt(4);
        int z = pos.z + random.nextInt(8) - random.nextInt(8);
        BlockPos newPos(x, y, z);

        if (level->isEmptyBlock(newPos) && level->getBlock(newPos.below()) == Blocks::GRASS) {
            level->setBlock(newPos, mBlock->defaultBlockState(), 2, false);
        }
    }

    return true;
}
