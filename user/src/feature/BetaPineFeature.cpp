#include "feature/BetaPineFeature.h"
#include "Minecraft.Core/BlockPos.h"
#include "Minecraft.World/Random.h"
#include "Minecraft.World/level/Level.h"
#include "Minecraft.World/level/block/Blocks.h"
#include "Minecraft.World/level/block/state/BlockState.h"
#include "Minecraft.World/level/material/Material.h"

BetaPineFeature::BetaPineFeature() : BetaTreeFeature() {}

bool BetaPineFeature::place(Level* level, Random& random, const BlockPos& pos) {
    int i = random.nextInt(5) + 7;
    int j = i + random.nextInt(2) - 3;
    int k = i - j;
    int m = 1 + random.nextInt(k + 1);
    bool bool1 = true;
    if (pos.y < 1 || pos.y + i + 1 > 128)
        return false;
    int n;
    for (n = pos.y; n <= pos.y + 1 + i && bool1; n++) {
        int i2 = 1;
        if (n - pos.y < j) {
            i2 = 0;
        } else {
            i2 = m;
        }
        for (int i3 = pos.x - i2; i3 <= pos.x + i2 && bool1; i3++) {
            for (int i4 = pos.z - i2; i4 <= pos.z + i2 && bool1; i4++) {
                if (n >= 0 && n < 128) {
                    Block* i7 = level->getBlock(BlockPos(i3, n, i4));
                    if (i7 != 0 && i7 != Blocks::LEAVES)
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
    int b = 0;
    int i1;
    for (i1 = pos.y + i; i1 >= pos.y + j; i1--) {
        for (int i2 = pos.x - b; i2 <= pos.x + b; i2++) {
            int i3 = i2 - pos.x;
            for (int i4 = pos.z - b; i4 <= pos.z + b; i4++) {
                int i5 = i4 - pos.z;
                if ((abs(i3) != b || abs(i5) != b || b <= 0) &&
                    !level->getBlockState(BlockPos(i2, i1, i4))->getMaterial()->isSolid()) {
                    level->setBlock(BlockPos(i2, i1, i4), Blocks::LEAVES->getBlockState(1), 2,
                                    false);
                }
            }
        }
        if (b >= 1 && i1 == pos.y + j + 1) {
            b--;
        } else if (b < m) {
            b++;
        }
    }
    for (i1 = 0; i1 < i - 1; i1++) {
        Block* i2 = level->getBlock(BlockPos(pos.x, pos.y + i1, pos.z));
        if (i2 == Blocks::AIR || i2 == Blocks::LEAVES)
            level->setBlock(BlockPos(pos.x, pos.y + i1, pos.z), Blocks::LOG->getBlockState(1), 2,
                            false);
    }
    return true;
}
