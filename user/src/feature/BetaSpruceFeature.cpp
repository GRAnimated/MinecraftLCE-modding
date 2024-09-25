#include "feature/BetaSpruceFeature.h"
#include "Minecraft.Core/BlockPos.h"
#include "Minecraft.World/Random.h"
#include "Minecraft.World/level/Level.h"
#include "Minecraft.World/level/block/Blocks.h"
#include "Minecraft.World/level/block/state/BlockState.h"
#include "Minecraft.World/level/material/Material.h"

BetaSpruceFeature::BetaSpruceFeature() : BetaTreeFeature() {}

bool BetaSpruceFeature::place(Level* level, Random& random, const BlockPos& pos) {
    int i = random.nextInt(4) + 6;
    int j = 1 + random.nextInt(2);
    int k = i - j;
    int m = 2 + random.nextInt(2);
    bool bool1 = true;
    if (pos.y < 1 || pos.y + i + 1 > 128)
        return false;
    int n;
    for (n = pos.y; n <= pos.y + 1 + i && bool1; n++) {
        int i4 = 1;
        if (n - pos.y < j) {
            i4 = 0;
        } else {
            i4 = m;
        }
        for (int i5 = pos.x - i4; i5 <= pos.x + i4 && bool1; i5++) {
            for (int i6 = pos.z - i4; i6 <= pos.z + i4 && bool1; i6++) {
                if (n >= 0 && n < 128) {
                    Block* i7 = level->getBlock(BlockPos(i5, n, i6));
                    if (i7 != Blocks::AIR && i7 != Blocks::LEAVES)
                        bool1 = false;
                } else {
                    bool1 = false;
                }
            }
        }
    }
    if (!bool1)
        return false;
    Block* n2 = level->getBlock(BlockPos(pos.x, pos.y - 1, pos.z));
    if ((n2 != Blocks::GRASS && n2 != Blocks::DIRT) || pos.y >= 128 - i - 1)
        return false;
    level->setBlock(BlockPos(pos.x, pos.y - 1, pos.z), Blocks::DIRT->defaultBlockState(), 2, false);
    int i1 = random.nextInt(2);
    int i2 = 1;
    bool bool2 = false;
    int i3;
    for (i3 = 0; i3 <= k; i3++) {
        int i4 = pos.y + i - i3;
        for (int i5 = pos.x - i1; i5 <= pos.x + i1; i5++) {
            int i6 = i5 - pos.x;
            for (int i7 = pos.z - i1; i7 <= pos.z + i1; i7++) {
                int i8 = i7 - pos.z;
                if ((abs(i6) != i1 || abs(i8) != i1 || i1 <= 0) &&
                    !level->getBlockState(BlockPos(i5, i4, i7))->getMaterial()->isSolid())
                    level->setBlock(BlockPos(i5, i4, i7), Blocks::LEAVES->getBlockState(1), 2,
                                    false);
            }
        }
        if (i1 >= i2) {
            i1 = bool2;
            bool2 = true;
            if (++i2 > m)
                i2 = m;
        } else {
            i1++;
        }
    }
    i3 = random.nextInt(3);
    for (int b = 0; b < i - i3; b++) {
        Block* i4 = level->getBlock(BlockPos(pos.x, pos.y + b, pos.z));
        if (i4 == Blocks::AIR || i4 == Blocks::LEAVES)
            level->setBlock(BlockPos(pos.x, pos.y + b, pos.z), Blocks::LOG->getBlockState(1), 2,
                            false);
    }
    return true;
}
