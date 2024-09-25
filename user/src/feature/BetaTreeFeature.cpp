#include "feature/BetaTreeFeature.h"
#include "Minecraft.Core/BlockPos.h"
#include "Minecraft.World/Random.h"
#include "Minecraft.World/level/Level.h"
#include "Minecraft.World/level/block/Blocks.h"
#include "Minecraft.World/level/block/state/BlockState.h"

BetaTreeFeature::BetaTreeFeature() : Feature(false) {}

bool BetaTreeFeature::place(Level* level, Random& random, const BlockPos& pos) {
    int treeHeight = random.nextInt(3) + 4;

    if (pos.y < 1 || pos.y + treeHeight + 1 > 128) {
        return false;
    }

    if (!canPlaceTree(level, pos, treeHeight)) {
        return false;
    }

    const Block* blockBelow = level->getBlockState(pos.below())->getBlock();
    if (blockBelow != Blocks::DIRT && blockBelow != Blocks::GRASS) {
        return false;
    }

    level->setBlock(pos.below(), Blocks::DIRT->defaultBlockState(), 2, false);

    generateLeaves(level, random, pos, treeHeight);
    generateTrunk(level, pos, treeHeight);

    return true;
}

bool BetaTreeFeature::canPlaceTree(Level* level, const BlockPos& pos, int height) {
    for (int y = pos.y; y <= pos.y + 1 + height; ++y) {
        int radius = (y == pos.y) ? 0 : (y >= pos.y + 1 + height - 2) ? 2 : 1;

        for (int x = pos.x - radius; x <= pos.x + radius; ++x) {
            for (int z = pos.z - radius; z <= pos.z + radius; ++z) {
                if (y >= 0 && y < 128) {
                    const Block* block = level->getBlockState(BlockPos(x, y, z))->getBlock();
                    if (block != nullptr && block != Blocks::AIR && block != Blocks::LEAVES) {
                        return false;
                    }
                } else {
                    return false;
                }
            }
        }
    }
    return true;
}

void BetaTreeFeature::generateLeaves(Level* level, Random& random, const BlockPos& pos,
                                     int height) {
    for (int y = pos.y - 3 + height; y <= pos.y + height; ++y) {
        int leafLayer = y - (pos.y + height);
        int radius = 1 - leafLayer / 2;

        for (int x = pos.x - radius; x <= pos.x + radius; ++x) {
            int xOffset = x - pos.x;
            for (int z = pos.z - radius; z <= pos.z + radius; ++z) {
                int zOffset = z - pos.z;
                if (std::abs(xOffset) != radius || std::abs(zOffset) != radius ||
                    (random.nextInt(2) != 0 && leafLayer != 0)) {
                    BlockPos leafPos(x, y, z);
                    const Block* block = level->getBlockState(leafPos)->getBlock();
                    if (block == Blocks::AIR || block == Blocks::LEAVES) {
                        level->setBlock(leafPos, Blocks::LEAVES->defaultBlockState(), 2, false);
                    }
                }
            }
        }
    }
}

void BetaTreeFeature::generateTrunk(Level* level, const BlockPos& pos, int height) {
    for (int y = 0; y < height; ++y) {
        BlockPos trunkPos(pos.x, pos.y + y, pos.z);
        const Block* block = level->getBlockState(trunkPos)->getBlock();
        if (block == nullptr || block == Blocks::AIR || block == Blocks::LEAVES) {
            level->setBlock(trunkPos, Blocks::LOG->defaultBlockState(), 2, false);
        }
    }
}
