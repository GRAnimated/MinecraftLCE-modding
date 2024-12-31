#include "biome/BetaBiomeDecorator.h"

#include "Minecraft.Core/BlockPos.h"
#include "Minecraft.World/Random.h"
#include "Minecraft.World/level/Level.h"
#include "Minecraft.World/level/biome/Biome.h"
#include "Minecraft.World/level/block/Blocks.h"
#include "Minecraft.World/level/levelgen/feature/LakeFeature.h"
#include "Minecraft.World/level/levelgen/feature/OreFeature.h"
#include "Minecraft.World/level/levelgen/feature/SpringFeature.h"
#include "Minecraft.World/level/levelgen/synth/PerlinNoise.h"
#include "biome/CustomBiomes.h"
#include "feature/BetaClayFeature.h"
#include "feature/BetaFlowerFeature.h"
#include "feature/BetaPineFeature.h"
#include "feature/BetaReedsFeature.h"
#include "feature/BetaSpringFeature.h"
#include "feature/BetaSpruceFeature.h"
#include "feature/BetaTallgrassFeature.h"
#include "feature/BetaTreeFeature.h"
#include "mallow/hook/helpers.hpp"
#include "noise/BetaPerlinNoise.h"
#include "world/BetaLevelSource.h"

BetaBiomeDecorator::BetaBiomeDecorator() : BiomeDecorator() {}

void BetaBiomeDecorator::decorate(Level* level, Random& random, Biome* biome, BlockPos const& pos) {
    // BiomeDecorator::decorate(level, random, biome, pos);

    double d = 0.25;

    /*
    for (k = 0; k < 8; k++) {
        int x = blockX + mRandom->nextInt(16) + 8;
        int y = mRandom->nextInt(128);
        int z = blockZ + mRandom->nextInt(16) + 8;
        (new MonsterRoomFeature()).place(source->mLevel, mRandom, BlockPos(x, y, z));
    }
    */

    if (random.nextInt(4) == 0) {
        int x = pos.x + random.nextInt(16) + 8;
        int y = random.nextInt(128);
        int z = pos.z + random.nextInt(16) + 8;
        (LakeFeature(Blocks::WATER)).place(level, random, BlockPos(x, y, z));
    }
    if (random.nextInt(8) == 0) {
        int x = pos.x + random.nextInt(16) + 8;
        int y = random.nextInt(random.nextInt(120) + 8);
        int z = pos.z + random.nextInt(16) + 8;
        if (y < 64 || random.nextInt(10) == 0)
            (LakeFeature(Blocks::LAVA)).place(level, random, BlockPos(x, y, z));
    }

    int k = 0;

    for (k = 0; k < 10; k++) {
        int x = pos.x + random.nextInt(16);
        int y = random.nextInt(128);
        int z = pos.z + random.nextInt(16);
        (BetaClayFeature(32)).place(level, random, BlockPos(x, y, z));
    }

    for (k = 0; k < 20; k++) {
        int x = pos.x + random.nextInt(16);
        int y = random.nextInt(128);
        int z = pos.z + random.nextInt(16);
        (OreFeature(Blocks::DIRT->defaultBlockState(), 32)).place(level, random, BlockPos(x, y, z));
    }
    for (k = 0; k < 10; k++) {
        int x = pos.x + random.nextInt(16);
        int y = random.nextInt(128);
        int z = pos.z + random.nextInt(16);
        (OreFeature(Blocks::GRAVEL->defaultBlockState(), 32))
            .place(level, random, BlockPos(x, y, z));
    }
    for (k = 0; k < 20; k++) {
        int x = pos.x + random.nextInt(16);
        int y = random.nextInt(128);
        int z = pos.z + random.nextInt(16);
        (OreFeature(Blocks::COAL_ORE->defaultBlockState(), 16))
            .place(level, random, BlockPos(x, y, z));
    }
    for (k = 0; k < 20; k++) {
        int x = pos.x + random.nextInt(16);
        int y = random.nextInt(64);
        int z = pos.z + random.nextInt(16);
        (OreFeature(Blocks::IRON_ORE->defaultBlockState(), 8))
            .place(level, random, BlockPos(x, y, z));
    }
    for (k = 0; k < 2; k++) {
        int x = pos.x + random.nextInt(16);
        int y = random.nextInt(32);
        int z = pos.z + random.nextInt(16);
        (OreFeature(Blocks::GOLD_ORE->defaultBlockState(), 8))
            .place(level, random, BlockPos(x, y, z));
    }
    for (k = 0; k < 8; k++) {
        int x = pos.x + random.nextInt(16);
        int y = random.nextInt(16);
        int z = pos.z + random.nextInt(16);
        (OreFeature(Blocks::REDSTONE_ORE->defaultBlockState(), 7))
            .place(level, random, BlockPos(x, y, z));
    }
    for (k = 0; k < 1; k++) {
        int x = pos.x + random.nextInt(16);
        int y = random.nextInt(16);
        int z = pos.z + random.nextInt(16);
        (OreFeature(Blocks::DIAMOND_ORE->defaultBlockState(), 7))
            .place(level, random, BlockPos(x, y, z));
    }
    for (k = 0; k < 1; k++) {
        int x = pos.x + random.nextInt(16);
        int y = random.nextInt(16) + random.nextInt(16);
        int z = pos.z + random.nextInt(16);
        (OreFeature(Blocks::LAPIS_ORE->defaultBlockState(), 6))
            .place(level, random, BlockPos(x, y, z));
    }

    PerlinNoise* forestNoise = BetaLevelSource::getInstance()->mForestNoise;

    double noiseValue = forestNoise->getValue(pos.x * d, pos.z * d);

    d = 0.5;
    k = (int)((noiseValue / 8.0 + random.nextDouble() * 4.0 + 4.0) / 3.0);

    int n;
    int m = 0;
    if (random.nextInt(10) == 0)
        m++;

    if (biome == CustomBiomes::FOREST)
        m += k + 5;
    if (biome == CustomBiomes::RAINFOREST)
        m += k + 5;
    if (biome == CustomBiomes::SEASONAL_FOREST)
        m += k + 2;
    if (biome == CustomBiomes::TAIGA)
        m += k + 5;
    if (biome == CustomBiomes::DESERT)
        m -= 20;
    if (biome == CustomBiomes::TUNDRA)
        m -= 20;
    if (biome == CustomBiomes::PLAINS)
        m -= 20;

    for (n = 0; n < m; n++) {
        int i2 = pos.x + random.nextInt(16) + 8;
        int i3 = pos.z + random.nextInt(16) + 8;
        BetaTreeFeature* feature = (BetaTreeFeature*)biome->getTreeFeature(random);
        feature->init();
        feature->place(level, random,
                       BlockPos(i2, (level->getHeightmapPos(BlockPos(i2, 0, i3))).y, i3));

        delete feature;
    }

    for (n = 0; n < 2; n++) {
        int i2 = pos.x + random.nextInt(16) + 8;
        int i3 = random.nextInt(128);
        int i4 = pos.z + random.nextInt(16) + 8;
        (BetaFlowerFeature(Blocks::YELLOW_FLOWER)).place(level, random, BlockPos(i2, i3, i4));
    }
    if (random.nextInt(2) == 0) {
        n = pos.x + random.nextInt(16) + 8;
        int i2 = random.nextInt(128);
        int i3 = pos.z + random.nextInt(16) + 8;
        (BetaFlowerFeature(Blocks::RED_FLOWER)).place(level, random, BlockPos(n, i2, i3));
    }
    if (random.nextInt(4) == 0) {
        n = pos.x + random.nextInt(16) + 8;
        int i2 = random.nextInt(128);
        int i3 = pos.z + random.nextInt(16) + 8;
        (BetaFlowerFeature(Blocks::BROWN_MUSHROOM)).place(level, random, BlockPos(n, i2, i3));
    }
    if (random.nextInt(8) == 0) {
        n = pos.x + random.nextInt(16) + 8;
        int i2 = random.nextInt(128);
        int i3 = pos.z + random.nextInt(16) + 8;
        (BetaFlowerFeature(Blocks::RED_MUSHROOM)).place(level, random, BlockPos(n, i2, i3));
    }

    int plantCount = 0;

    if (biome == CustomBiomes::FOREST) {
        plantCount = 2;
    }

    if (biome == CustomBiomes::RAINFOREST) {
        plantCount = 10;
    }

    if (biome == CustomBiomes::SEASONAL_FOREST) {
        plantCount = 2;
    }

    if (biome == CustomBiomes::TAIGA) {
        plantCount = 1;
    }

    if (biome == CustomBiomes::PLAINS) {
        plantCount = 10;
    }

    for (int i = 0; i < plantCount; ++i) {
        const BlockState* block = Blocks::TALLGRASS->getBlockState(1);
        if (biome == CustomBiomes::RAINFOREST && random.nextInt(3) != 0) {
            block = Blocks::TALLGRASS->getBlockState(2);
        }

        int x = pos.x + random.nextInt(16) + 8;
        int y = random.nextInt(128);
        int z = pos.z + random.nextInt(16) + 8;

        BetaTallgrassFeature(block).place(level, random, BlockPos(x, y, z));
    }

    /*
    if (biome == CustomBiomes::DESERT) {
        for (int i = 0; i < 2; ++i) {
            int x = pos.x + random.nextInt(16) + 8;
            int y = random.nextInt(128);
            int z = pos.z + random.nextInt(16) + 8;

            new WorldGenDeadBush().generate(world, random, mutablePos.setPos(x, y, z));
        }
    }
    */

    for (n = 0; n < 10; n++) {
        int i2 = pos.x + random.nextInt(16) + 8;
        int i3 = random.nextInt(128);
        int i4 = pos.z + random.nextInt(16) + 8;
        (BetaReedsFeature()).place(level, random, BlockPos(i2, i3, i4));
    }

    int i1 = 0;
    for (i1 = 0; i1 < 50; i1++) {
        int i2 = pos.x + random.nextInt(16) + 8;
        int i3 = random.nextInt(random.nextInt(120) + 8);
        int i4 = pos.z + random.nextInt(16) + 8;
        (BetaSpringFeature(Blocks::FLOWING_WATER)).place(level, random, BlockPos(i2, i3, i4));
        //(SpringFeature(Blocks::WATER, false)).place(level, random, BlockPos(i2, i3, i4));
    }
    for (i1 = 0; i1 < 20; i1++) {
        int i2 = pos.x + random.nextInt(16) + 8;
        int i3 = random.nextInt(random.nextInt(random.nextInt(112) + 8) + 8);
        int i4 = pos.z + random.nextInt(16) + 8;
        (BetaSpringFeature(Blocks::FLOWING_LAVA)).place(level, random, BlockPos(i2, i3, i4));
        //(SpringFeature(Blocks::LAVA, false)).place(level, random, BlockPos(i2, i3, i4));
    }
    /*

    if (mRandom->nextInt(32) == 0) {
        n = blockX + mRandom->nextInt(16) + 8;
        int i2 = mRandom->nextInt(128);
        int i3 = blockZ + mRandom->nextInt(16) + 8;
        (new PumpkinFeature()).place(source->mLevel, mRandom, n, i2, i3);
    }
    n = 0;
    if (biome == Biomes::desert)
        n += 10;
    int i1;
    for (i1 = 0; i1 < n; i1++) {
        int i2 = blockX + mRandom->nextInt(16) + 8;
        int i3 = mRandom->nextInt(128);
        int i4 = blockZ + mRandom->nextInt(16) + 8;
        (new CactusFeature()).place(source->mLevel, mRandom, BlockPos(x, y, z));
    }
    */
}

void BetaBiomeDecorator::decorate(Biome* biome, Level* level, Random& random) {
    // BiomeDecorator::decorate(biome, level, random);
}

void BetaBiomeDecorator::decorateOres(Level* level, Random& random) {
    // BiomeDecorator::decorateOres(level, random);
}

struct BiomeCreateDecoratorHook : mallow::hook::Trampoline<BiomeCreateDecoratorHook> {
    static BiomeDecorator* Callback() {
        // return Orig();
        return new BetaBiomeDecorator();
    }
};

struct BiomeDecoratorDecorateHook : mallow::hook::Trampoline<BiomeDecoratorDecorateHook> {
    static void Callback(BiomeDecorator* decorator, Level* level, Random& random, Biome* biome,
                         BlockPos const& pos) {
        // decorator->decorate(level, random, biome, pos);
    }
};

void BetaBiomeDecorator::initHooks() {
    // BiomeCreateDecoratorHook::InstallAtOffset(0x425D4);
    // BiomeDecoratorDecorateHook::InstallAtOffset(0x445F0);
}
