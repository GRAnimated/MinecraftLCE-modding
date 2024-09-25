#include "feature/BetaReedsFeature.h"

#include "Minecraft.Core/BlockPos.h"
#include "Minecraft.World/Random.h"
#include "Minecraft.World/level/Level.h"
#include "Minecraft.World/level/block/Blocks.h"
#include "Minecraft.World/level/block/state/BlockState.h"
#include "Minecraft.World/level/material/Material.h"

// TODO: Move this to its own file
class ReedsBlock : public Block {
public:
    bool canSurvive(Level*, BlockPos const&);
};

BetaReedsFeature::BetaReedsFeature() : Feature(false) {}

bool BetaReedsFeature::place(Level* level, Random& random, const BlockPos& pos) {
    for (int b = 0; b < 20; b++) {
        int x = pos.x + random.nextInt(4) - random.nextInt(4);
        int y = pos.y;
        int z = pos.z + random.nextInt(4) - random.nextInt(4);
        if (level->isEmptyBlock(BlockPos(x, y, z)) &&
            (level->getBlockState(BlockPos(x - 1, y - 1, z))->getMaterial() ==
                 (Material*)Material::WATER ||
             level->getBlockState(BlockPos(x + 1, y - 1, z))->getMaterial() ==
                 (Material*)Material::WATER ||
             level->getBlockState(BlockPos(x, y - 1, z - 1))->getMaterial() ==
                 (Material*)Material::WATER ||
             level->getBlockState(BlockPos(x, y - 1, z + 1))->getMaterial() ==
                 (Material*)Material::WATER) &&
            level->getBlockState(BlockPos(x, y - 1, z))->getMaterial() !=
                (Material*)Material::WATER &&
            level->getBlockState(BlockPos(x, y - 1, z))->getBlock() != Blocks::ICE) {
            int m = 2 + random.nextInt(random.nextInt(3) + 1);
            for (int b1 = 0; b1 < m; b1++) {
                ReedsBlock* block = (ReedsBlock*)level->getBlock(BlockPos(x, y + b1, z));
                if (block->canSurvive(level, BlockPos(x, y + b1, z)))
                    level->setBlock(BlockPos(x, y + b1, z), Blocks::REEDS->defaultBlockState(), 2,
                                    false);
            }
        }
    }
    return true;
}
