#include "world/CustomLevelSource.h"

#include <exl/patch/code_patcher.hpp>
#include <mallow/hook/helpers.hpp>
#include "Minecraft.World/level/biome/BiomeDecorator.h"
#include "Minecraft.World/level/levelgen/feature/LargeFeature.h"
#include "Minecraft.World/level/levelgen/synth/PerlinNoise.h"
#include "Minecraft.World/level/storage/LevelData.h"
#include "windows.h"

#include "Minecraft.Core/BlockPos.h"
#include "Minecraft.World/ArrayWithLength.h"
#include "Minecraft.World/Random.h"
#include "Minecraft.World/level/Level.h"
#include "Minecraft.World/level/biome/Biome.h"
#include "Minecraft.World/level/biome/BiomeSource.h"
#include "Minecraft.World/level/block/Blocks.h"
#include "Minecraft.World/level/block/state/BlockState.h"
#include "Minecraft.World/level/chunk/LevelChunk.h"
#include "Minecraft.World/level/levelgen/ChunkPrimer.h"
#include "Minecraft.World/level/levelgen/OverworldLevelSource.h"
#include "Minecraft.World/level/levelgen/feature/ClayFeature.h"
#include "Minecraft.World/level/levelgen/feature/LakeFeature.h"
#include "Minecraft.World/level/levelgen/feature/SpringFeature.h"
#include "Minecraft.World/level/levelgen/synth/PerlinSimplexNoise.h"
#include "Minecraft.World/level/material/Material.h"

#include "biome/BetaBiome.h"
#include "biome/CustomBiomes.h"
#include "feature/BetaSpringFeature.h"

void CustomLevelSource::create(OverworldLevelSource* source, long long seed,
                               bool generateMapFeatures, void* config) {
    BetaBiome::recalc();

    hCustomLevelSource = new CustomLevelSource(source, seed, generateMapFeatures, config);
}

CustomLevelSource::CustomLevelSource(OverworldLevelSource* source, long long seed,
                                     bool generateMapFeatures, void* config)
    : mSource(source), mSeed(seed), mGenerateMapFeatures(generateMapFeatures), mConfig(config) {
    mRandom = new Random();

    // source->mLevel->setSeaLevel(63);
    // source->mLevel->getLevelData()->setCloudHeight(108);

    seed = seed = 1772835215;
    mSeed = seed;

    // source->mLevel->getLevelData()->setSeed(seed);

    // seed = 8091867987493326313;
    // mSeed = seed;

    // seed = 3257840388504953787;
    // mSeed = seed;

    // seed = -1623774494;
    // mSeed = seed;

    mBiomeSource = new BetaBiomeSource(source->mLevel, seed);

    mRandom->setSeed(seed);

    // mNoise = new PerlinNoise(mRandom, 3);

    lperlinNoise1 = new PerlinNoise(mRandom, 16);
    lperlinNoise2 = new PerlinNoise(mRandom, 16);
    perlinNoise1 = new PerlinNoise(mRandom, 8);
    perlinNoise2 = new PerlinNoise(mRandom, 4);
    perlinNoise3 = new PerlinNoise(mRandom, 4);
    scaleNoise = new PerlinNoise(mRandom, 10);
    depthNoise = new PerlinNoise(mRandom, 16);
    forestNoise = new PerlinNoise(mRandom, 8);

    biomes = arrayWithLength<Biome*>(256, true);
}

CustomLevelSource* CustomLevelSource::getInstance() {
    return hCustomLevelSource;
}

LevelChunk* CustomLevelSource::createChunk(int chunkX, int chunkZ) {
    mRandom->setSeed((chunkX * 341873128712L) + (chunkZ * 132897987541L));

    void* ids = XPhysicalAlloc(0x8000, 0xFFFFFFFFFFFFFFFFLL, 0x1000uLL, 4u);
    XMemSet128(ids, 0, 0x8000u);
    arrayWithLength<uchar> blockIds((uchar*)ids, 0x8000u);

    void* data = XPhysicalAlloc(0x4000, 0xFFFFFFFFFFFFFFFFLL, 0x1000uLL, 4u);
    XMemSet128(data, 0, 0x4000u);
    arrayWithLength<uchar> blockData((uchar*)data, 0x4000u);

    biomes = mBiomeSource->getBiomeBlock(chunkX * 16, chunkZ * 16, 16, 16);  // update arrays

    ChunkPrimer primer = ChunkPrimer(false, blockIds, blockData);
    prepareHeights(chunkX, chunkZ, &primer);
    buildSurfaces(chunkX, chunkZ, &primer);

    mSource->mCaveFeature->apply(mSource->mLevel, chunkX, chunkZ, &primer);
    // mSource->mRavineFeature->apply(mSource->mLevel, chunkX, chunkZ, &primer);

    LevelChunk* chunk = new LevelChunk(mSource->mLevel, &primer, chunkX, chunkZ);

    XPhysicalFree(ids);
    XPhysicalFree(data);

    return chunk;
}

double clamp(double value, double min, double max) {
    if (value < min)
        return min;
    if (value > max)
        return max;
    return value;
}

double lerp(double start, double end, double t) {
    return start + t * (end - start);
}

double CustomLevelSource::applySlides(double density, int noiseY) {
    const int topSlideTarget = -10;
    const int topSlideSize = 3;
    const int topSlideOffset = 0;
    const int bottomSlideTarget = -10;
    const int bottomSlideSize = 3;
    const int bottomSlideOffset = 0;
    const int noiseSizeY = 64;

    if (topSlideSize > 0.0) {
        double delta = ((double)(noiseSizeY - noiseY) - topSlideOffset) / topSlideSize;
        delta = clamp(delta, 0.0, 1.0);
        density = lerp(topSlideTarget, density, delta);
    }

    if (bottomSlideSize > 0.0) {
        double delta = ((double)noiseY - bottomSlideOffset) / bottomSlideSize;
        delta = clamp(delta, 0.0, 1.0);
        density = lerp(bottomSlideTarget, density, delta);
    }

    return density;
}

void CustomLevelSource::prepareHeights(int chunkX, int chunkZ, ChunkPrimer* primer) {
    int b1 = 4;
    int seaLevel = 64;
    // int i = b1 + 1;
    int b3 = 17;
    int j = b1 + 1;

    arrayWithLength<double> heights = arrayWithLength<double>(0x1A9, true);

    heights = getHeights(heights, b1 * chunkX, 0, b1 * chunkZ, 5, 17, 5);

    for (int b4 = 0; b4 < b1; b4++) {
        for (int b = 0; b < b1; b++) {
            for (int b5 = 0; b5 < 16; b5++) {
                double d1 = 0.125;
                double d2 = heights[((b4 + 0) * j + b + 0) * b3 + b5 + 0];
                double d3 = heights[((b4 + 0) * j + b + 1) * b3 + b5 + 0];
                double d4 = heights[((b4 + 1) * j + b + 0) * b3 + b5 + 0];
                double d5 = heights[((b4 + 1) * j + b + 1) * b3 + b5 + 0];
                double d6 = (heights[((b4 + 0) * j + b + 0) * b3 + b5 + 1] - d2) * d1;
                double d7 = (heights[((b4 + 0) * j + b + 1) * b3 + b5 + 1] - d3) * d1;
                double d8 = (heights[((b4 + 1) * j + b + 0) * b3 + b5 + 1] - d4) * d1;
                double d9 = (heights[((b4 + 1) * j + b + 1) * b3 + b5 + 1] - d5) * d1;
                for (int b6 = 0; b6 < 8; b6++) {
                    double d10 = 0.25;
                    double d11 = d2;
                    double d12 = d3;
                    double d13 = (d4 - d2) * d10;
                    double d14 = (d5 - d3) * d10;
                    for (int b7 = 0; b7 < 4; b7++) {
                        int k = (b7 + b4 * 4) << 11 | (0 + b * 4) << 7 | b5 * 8 + b6;

                        double d15 = 0.25;
                        double d16 = d11;
                        double d17 = (d12 - d11) * d15;
                        for (int b8 = 0; b8 < 4; b8++) {
                            /*
                            double d = mBiomeSource->temperatures[(b7 + b4 * 4) * 16 + b * 4 + b8];
                            const BlockState* block = Blocks::AIR->defaultBlockState();
                            if (b5 * 8 + b6 < seaLevel) {
                                if (d < 0.5 && b5 * 8 + b6 >= seaLevel - 1) {
                                    block = Blocks::ICE->defaultBlockState();
                                } else {
                                    block = Blocks::WATER->defaultBlockState();
                                }
                            }
                            */
                            const BlockState* block = Blocks::AIR->defaultBlockState();
                            if (b5 * 8 + b6 < seaLevel) {
                                block = Blocks::WATER->defaultBlockState();
                            }
                            if (d16 > 0.0)
                                block = Blocks::STONE->defaultBlockState();
                            primer->setState(k, block);
                            k += 128;
                            d16 += d17;
                        }
                        d11 += d13;
                        d12 += d14;
                    }
                    d2 += d6;
                    d3 += d7;
                    d4 += d8;
                    d5 += d9;
                }
            }
        }
    }
    if (heights.data) {
        delete[] reinterpret_cast<double*>(heights.data);
    }
}

arrayWithLength<double> CustomLevelSource::getHeights(arrayWithLength<double> heights, int posX,
                                                      int posY, int posZ, int width, int depth,
                                                      int length) {
    if (heights.data == nullptr || int(heights.length) < width * depth * length)
        heights = arrayWithLength<double>(width * depth * length, true);

    arrayWithLength<double> sr = arrayWithLength<double>();
    arrayWithLength<double> dr = arrayWithLength<double>();
    arrayWithLength<double> pnr = arrayWithLength<double>();
    arrayWithLength<double> ar = arrayWithLength<double>();
    arrayWithLength<double> br = arrayWithLength<double>();

    double d1 = 684.412;
    double d2 = 684.412;

    arrayWithLength<double> temperatures = mBiomeSource->temperatures;
    arrayWithLength<double> downfalls = mBiomeSource->downfalls;

    sr = scaleNoise->getRegion(sr, posX, posZ, width, length, 1.121, 1.121, 0.5);
    dr = depthNoise->getRegion(dr, posX, posZ, width, length, 200.0, 200.0, 0.5);
    pnr = perlinNoise1->getRegion(pnr, posX, posY, posZ, width, depth, length, d1 / 80.0,
                                  d2 / 160.0, d1 / 80.0);
    ar = lperlinNoise1->getRegion(ar, posX, posY, posZ, width, depth, length, d1, d2, d1);
    br = lperlinNoise2->getRegion(br, posX, posY, posZ, width, depth, length, d1, d2, d1);
    int b1 = 0;
    int b2 = 0;
    int i = 16 / width;
    for (int b3 = 0; b3 < width; b3++) {
        int j = b3 * i + i / 2;
        for (int b = 0; b < length; b++) {
            int k = b * i + i / 2;
            double d3 = temperatures[j * 16 + k];
            double d4 = downfalls[j * 16 + k] * d3;
            double d5 = 1.0 - d4;
            d5 *= d5;
            d5 *= d5;
            d5 = 1.0 - d5;
            double d6 = (sr[b2] + 256.0) / 512.0;
            d6 *= d5;
            if (d6 > 1.0)
                d6 = 1.0;
            double d7 = dr[b2] / 8000.0;
            if (d7 < 0.0)
                d7 = -d7 * 0.3;
            d7 = d7 * 3.0 - 2.0;
            if (d7 < 0.0) {
                d7 /= 2.0;
                if (d7 < -1.0)
                    d7 = -1.0;
                d7 /= 1.4;
                d7 /= 2.0;
                d6 = 0.0;
            } else {
                if (d7 > 1.0)
                    d7 = 1.0;
                d7 /= 8.0;
            }
            if (d6 < 0.0)
                d6 = 0.0;
            d6 += 0.5;
            d7 = d7 * depth / 16.0;
            double d8 = depth / 2.0 + d7 * 4.0;
            b2++;
            for (int b4 = 0; b4 < depth; b4++) {
                double d9 = 0.0;
                double d10 = (b4 - d8) * 12.0 / d6;
                if (d10 < 0.0)
                    d10 *= 4.0;
                double d11 = ar[b1] / 512.0;
                double d12 = br[b1] / 512.0;
                double d13 = (pnr[b1] / 10.0 + 1.0) / 2.0;
                if (d13 < 0.0) {
                    d9 = d11;
                } else if (d13 > 1.0) {
                    d9 = d12;
                } else {
                    d9 = d11 + (d12 - d11) * d13;
                }
                d9 -= d10;
                /*
                ORIGINAL
                if (b4 > depth - 4) {
                    double d = ((b4 - depth - 4) / 3.0F);
                    d9 = d9 * (1.0 - d) + -10.0 * d;
                }
                */
                double falloffStart = 0.6;  // Adjust this value to control where the falloff
                                            // begins (closer to 1 = earlier cutoff)
                if (b4 > depth * falloffStart) {
                    double falloff =
                        (b4 - depth * falloffStart) /
                        (depth * (1.0 - falloffStart));  // Scale falloff over the top portion
                    falloff = std::min(falloff, 1.0);    // Clamp to 1.0
                    d9 = d9 * (1.0 - falloff) + -10.0 * falloff;
                }

                heights[b1] = d9;
                b1++;
            }
        }
    }

    if (sr.data) {
        delete[] reinterpret_cast<double*>(sr.data);
    }
    if (dr.data) {
        delete[] reinterpret_cast<double*>(dr.data);
    }
    if (pnr.data) {
        delete[] reinterpret_cast<double*>(pnr.data);
    }
    if (ar.data) {
        delete[] reinterpret_cast<double*>(ar.data);
    }
    if (br.data) {
        delete[] reinterpret_cast<double*>(br.data);
    }

    return heights;
}

void CustomLevelSource::buildSurfaces(int chunkX, int chunkZ, ChunkPrimer* primer) {
    int sandCutoff = 64;

    double scale = 0.03125;

    int blockX = chunkX * 16;
    int blockZ = chunkZ * 16;

    arrayWithLength<double> sandBuffer = arrayWithLength<double>(256, true);
    arrayWithLength<double> gravelBuffer = arrayWithLength<double>(256, true);
    arrayWithLength<double> depthBuffer = arrayWithLength<double>(256, true);

    perlinNoise2->getRegion(sandBuffer, blockX, blockZ, 1.0, 16, 16, 1, scale, scale, 1.0);
    perlinNoise2->getRegion(gravelBuffer, blockZ, (int)109.0134, blockX, 16, 1, 16, scale, 1.0,
                            scale);
    perlinNoise3->getRegion(depthBuffer, blockX, blockZ, 1.0, 16, 16, 1, scale * 2.0, scale * 2.0,
                            scale * 2.0);
    arrayWithLength<Biome*> biomes = mBiomeSource->nonDestructiveGetBiomeBlock(
        arrayWithLength<Biome*>(), blockX, blockZ, 16, 16);
    for (int z = 0; z < 16; z++) {
        for (int x = 0; x < 16; x++) {
            bool isSandBeach = (sandBuffer[z * 16 + x] + mRandom->nextDouble() * 0.2 > 0.0);
            bool isGravelBeach = (gravelBuffer[z * 16 + x] + mRandom->nextDouble() * 0.2 > 3.0);

            int depth = (int)(depthBuffer[z * 16 + x] / 3.0 + 3.0 + mRandom->nextDouble() * 0.25);
            int runDepth = -1;

            const BlockState* topBlock = Blocks::GRASS->defaultBlockState();
            const BlockState* fillerBlock = Blocks::DIRT->defaultBlockState();

            if (biomes[x + z * 16] == CustomBiomes::DESERT) {
                topBlock = Blocks::SAND->defaultBlockState();
                fillerBlock = Blocks::SAND->defaultBlockState();
            }

            const BlockState* airBlock = Blocks::AIR->defaultBlockState();
            const BlockState* stoneBlock = Blocks::STONE->defaultBlockState();
            const BlockState* bedrockBlock = Blocks::BEDROCK->defaultBlockState();
            const BlockState* sandBlock = Blocks::SAND->defaultBlockState();
            const BlockState* gravelBlock = Blocks::GRAVEL->defaultBlockState();

            for (int y = 127; y >= 0; y--) {
                int k = (z * 16 + x) * 128 + y;
                if (y <= 0 + mRandom->nextInt(5)) {
                    primer->setState(k, bedrockBlock);
                } else {
                    const BlockState* currentBlock = primer->getState(k);
                    if (currentBlock == airBlock) {
                        runDepth = -1;
                    } else if (currentBlock == stoneBlock) {
                        if (runDepth == -1) {
                            if (depth <= 0) {
                                topBlock = airBlock;
                                fillerBlock = stoneBlock;
                            } else if (y >= sandCutoff - 4 && y <= sandCutoff + 1) {
                                if (isGravelBeach) {
                                    topBlock = airBlock;
                                    fillerBlock = gravelBlock;
                                }
                                if (isSandBeach) {
                                    topBlock = sandBlock;
                                    fillerBlock = sandBlock;
                                }
                            }
                            if (y < sandCutoff && topBlock == airBlock)
                                topBlock = Blocks::WATER->defaultBlockState();

                            runDepth = depth;

                            if (y >= sandCutoff - 1) {
                                primer->setState(k, topBlock);
                            } else {
                                primer->setState(k, fillerBlock);
                            }

                        } else if (runDepth > 0) {
                            runDepth--;
                            primer->setState(k, fillerBlock);
                        }

                        if (runDepth == 0 && fillerBlock == sandBlock) {
                            runDepth = mRandom->nextInt(4);
                            fillerBlock = Blocks::SANDSTONE->defaultBlockState();
                        }
                    }
                }
            }
        }
    }

    if (sandBuffer.data) {
        delete[] reinterpret_cast<double*>(sandBuffer.data);
    }
    if (gravelBuffer.data) {
        delete[] reinterpret_cast<double*>(gravelBuffer.data);
    }
    if (depthBuffer.data) {
        delete[] reinterpret_cast<double*>(depthBuffer.data);
    }
}

int getTopSolidBlock(Level* level, int paramInt1, int paramInt2) {
    LevelChunk* levelChunk = level->getChunkAt(BlockPos(paramInt1, 64, paramInt2));
    int b;
    for (b = 127; level->getBlock(BlockPos(paramInt1, b, paramInt2))
                      ->defaultBlockState()
                      ->getMaterial()
                      ->blocksMotion() &&
                  b > 0;
         b--)
        ;
    paramInt1 &= 0xF;
    paramInt2 &= 0xF;
    while (b > 0) {
        const BlockState* state = levelChunk->getBlockState(BlockPos(paramInt1, b, paramInt2));
        if (state == Blocks::AIR->defaultBlockState() ||
            (!state->getMaterial()->blocksMotion() && !state->getMaterial()->isLiquid())) {
            b--;
            continue;
        }
        return b + 1;
    }
    return -1;
}

void CustomLevelSource::postProcess(int chunkX, int chunkZ) {
    // SandTile.instaFall = true;
    int blockX = chunkX * 16;
    int blockZ = chunkZ * 16;

    // Biome* biome = CustomBiomes::SEASONAL_FOREST;

    arrayWithLength<Biome*> biomes = arrayWithLength<Biome*>(256, true);

    biomes = mBiomeSource->nonDestructiveGetBiomeBlock(biomes, blockX + 16, blockZ + 16, 16, 16);

    Biome* biome = biomes[0];

    // Biome biome = source->mLevel->getBiomeSource()->getBiome(i + 16, j + 16);
    // Biome* biome = biomes[(i + 16) * (j * 16)];
    // Biome* biome = Biomes::FOREST;
    // Biome* biome = mBiomeSource->getBiome(*decoratePos);

    mRandom->setSeed(mSeed);
    long l1 = mRandom->nextLong() / 2L * 2L + 1L;
    long l2 = mRandom->nextLong() / 2L * 2L + 1L;
    mRandom->setSeed(chunkX * l1 + chunkZ * l2 ^ mSeed);

    BlockPos* decoratePos = new BlockPos(blockX, 0, blockZ);
    // Biome* biome = mSource->mLevel->getBiome(*decoratePos);
    //  Biome* biome = mBiomeSource->getBiome(chunkX + 16, chunkZ + 16);

    // biomes[blockX + blockZ * 16]->decorate(mSource->mLevel, *mRandom, decoratePos);
    biome->decorate(mSource->mLevel, *mRandom, decoratePos);
    delete decoratePos;

    // double d = 0.25;

    // int k;

    /*
    for (k = 0; k < 8; k++) {
        int x = blockX + mRandom->nextInt(16) + 8;
        int y = mRandom->nextInt(128);
        int z = blockZ + mRandom->nextInt(16) + 8;
        (new MonsterRoomFeature()).place(source->mLevel, mRandom, BlockPos(x, y, z));
    }



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

    arrayWithLength<double> temps = mBiomeSource->getTemperatureBlock(
        arrayWithLength<double>(), blockX + 8, blockZ + 8, 16, 16);

    for (int x = blockX + 8; x < blockX + 8 + 16; x++) {
        for (int z = blockZ + 8; z < blockZ + 8 + 16; z++) {
            int localX = x - (blockX + 8);
            int localZ = z - (blockZ + 8);

            int y = getTopSolidBlock(mSource->mLevel, x, z);

            double d1 = temps[localX * 16 + localZ] - (y - 64) / 64.0 * 0.3;
            if (d1 < 0.5 && y > 0 && y < 128 && mSource->mLevel->isEmptyBlock(BlockPos(x, y, z)) &&
                mSource->mLevel->getBlock(BlockPos(x, y - 1, z))
                    ->defaultBlockState()
                    ->getMaterial()
                    ->blocksMotion() &&
                mSource->mLevel->getBlock(BlockPos(x, y - 1, z))
                        ->defaultBlockState()
                        ->getMaterial() != Material::ICE)
                mSource->mLevel->setBlock(BlockPos(x, y, z),
                                          Blocks::SNOW_LAYER->defaultBlockState(), 0, 0);
        }
    }

    if (temps.data) {
        delete[] reinterpret_cast<double*>(temps.data);
    }

    // SandTile.instaFall = false;
}

struct CustomLevelSourceCreateHook : mallow::hook::Trampoline<CustomLevelSourceCreateHook> {
    static void Callback(OverworldLevelSource* source, long long seed, bool generateMapFeatures,
                         void* config) {
        // seed = 1772835215;

        // seed = 8091867987493326313;

        Orig(source, seed, generateMapFeatures, config);

        CustomLevelSource::create(source, seed, generateMapFeatures, config);
    }
};

struct CustomLevelSourceCreateChunkHook
    : mallow::hook::Trampoline<CustomLevelSourceCreateChunkHook> {
    static void Callback(OverworldLevelSource* source, int chunkX, int chunkZ) {
        // Orig(chunkX, chunkZ);

        hCustomLevelSource->createChunk(chunkX, chunkZ);
    }
};

struct CustomLevelSourcePostProcessHook
    : mallow::hook::Trampoline<CustomLevelSourcePostProcessHook> {
    static void Callback(OverworldLevelSource* source, int chunkX, int chunkZ) {
        // Orig(source, chunkX, chunkZ);

        hCustomLevelSource->postProcess(chunkX, chunkZ);
    }
};

struct LevelGetBiomeHook : mallow::hook::Trampoline<LevelGetBiomeHook> {
    static Biome* Callback(Level* level, const BlockPos& pos) {
        // return Orig(level, pos);

        // BetaBiome::recalc();
        //
        // double temperature = hCustomLevelSource->mBiomeSource->getTemperature(pos.x, pos.z);
        // double downfall = hCustomLevelSource->mBiomeSource->getDownfall(pos.x, pos.z);
        //
        // Biome* biome = BetaBiome::getBiome(temperature, downfall);
        // if (!biome)
        //    return CustomBiomes::PLAINS;
        // return biome;

        return CustomBiomes::RAINFOREST;
    }
};

struct ShoreLayerIsMesaFix : mallow::hook::Trampoline<ShoreLayerIsMesaFix> {
    static bool Callback(void* shoreLayer, int unk) { return false; }
};

void CustomLevelSource::initHooks() {
    CustomLevelSourceCreateHook::InstallAtOffset(0x286C88);
    CustomLevelSourceCreateChunkHook::InstallAtOffset(0x28825C);

    CustomLevelSourcePostProcessHook::InstallAtOffset(0x288688);

    LevelGetBiomeHook::InstallAtOffset(0x1F95A0);

    ShoreLayerIsMesaFix::InstallAtOffset(0x317A14);
}
