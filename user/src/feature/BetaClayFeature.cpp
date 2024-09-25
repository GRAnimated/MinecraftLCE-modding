#include "feature/BetaClayFeature.h"

#include <cmath>

#include "Minecraft.Core/BlockPos.h"
#include "Minecraft.World/Random.h"
#include "Minecraft.World/level/Level.h"
#include "Minecraft.World/level/block/Blocks.h"
#include "Minecraft.World/level/block/state/BlockState.h"
#include "Minecraft.World/level/material/Material.h"

BetaClayFeature::BetaClayFeature(int count) : Feature(false), mCount(count) {}

bool BetaClayFeature::place(Level* level, Random& random, const BlockPos& pos) {
    if (level->getBlockState(pos)->getMaterial() != (Material*)Material::WATER) {
        return false;
    }

    float angle = random.nextFloat() * M_PI;
    double x1 = pos.x + 8 + std::sin(angle) * mCount / 8.0F;
    double x2 = pos.x + 8 - std::sin(angle) * mCount / 8.0F;
    double z1 = pos.z + 8 + std::cos(angle) * mCount / 8.0F;
    double z2 = pos.z + 8 - std::cos(angle) * mCount / 8.0F;
    double y1 = pos.y + random.nextInt(3) + 2;
    double y2 = pos.y + random.nextInt(3) + 2;

    for (int i = 0; i <= mCount; i++) {
        double dx = x1 + (x2 - x1) * i / mCount;
        double dy = y1 + (y2 - y1) * i / mCount;
        double dz = z1 + (z2 - z1) * i / mCount;
        double radius = random.nextDouble() * mCount / 16.0;
        double radiusXZ = (std::sin(i * M_PI / mCount) + 1.0F) * radius + 1.0;
        double radiusY = (std::sin(i * M_PI / mCount) + 1.0F) * radius + 1.0;

        for (int x = int(dx - radiusXZ / 2.0); x <= int(dx + radiusXZ / 2.0); ++x) {
            for (int y = int(dy - radiusY / 2.0); y <= int(dy + radiusY / 2.0); ++y) {
                for (int z = int(dz - radiusXZ / 2.0); z <= int(dz + radiusXZ / 2.0); ++z) {
                    double distX = (x + 0.5 - dx) / (radiusXZ / 2.0);
                    double distY = (y + 0.5 - dy) / (radiusY / 2.0);
                    double distZ = (z + 0.5 - dz) / (radiusXZ / 2.0);

                    if (distX * distX + distY * distY + distZ * distZ < 1.0) {
                        Block* block = level->getBlock(BlockPos(x, y, z));
                        if (block == Blocks::SAND) {
                            level->setBlock(BlockPos(x, y, z), Blocks::CLAY->defaultBlockState(), 2,
                                            false);
                        }
                    }
                }
            }
        }
    }

    return true;
}
