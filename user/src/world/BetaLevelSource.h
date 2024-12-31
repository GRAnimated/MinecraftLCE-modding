#pragma once

#include "Minecraft.World/ArrayWithLength.h"
#include "biome/BetaBiomeSource.h"
// #include "noise/PerlinNoise.h"

class Biome;
class ChunkPrimer;
class LevelChunk;
class OverworldLevelSource;
class Random;
class PerlinNoise;
class PerlinSimplexNoise;

class BetaLevelSource {
public:
    static void create(OverworldLevelSource* source, long long seed, bool generateMapFeatures,
                       void* config);

    static BetaLevelSource* getInstance();

    BetaLevelSource(OverworldLevelSource* source, long long seed, bool generateMapFeatures,
                    void* config);

    LevelChunk* createChunk(int chunkX, int chunkZ);

    static double applySlides(double density, int noiseY);

    void prepareHeights(int chunkX, int chunkZ, ChunkPrimer* primer);
    arrayWithLength<double> getHeights(arrayWithLength<double> heights, int posX, int posY,
                                       int posZ, int width, int depth, int length);
    void buildSurfaces(int chunkX, int chunkZ, ChunkPrimer* primer);
    void postProcess(int chunkX, int chunkZ);

    static void initHooks();

    // private:
    OverworldLevelSource* mSource;
    BetaBiomeSource* mBiomeSource;
    long long mSeed;
    bool mGenerateMapFeatures;
    void* mConfig;

    Random* mRandom;

    PerlinNoise* lperlinNoise1;
    PerlinNoise* lperlinNoise2;
    PerlinNoise* perlinNoise1;
    PerlinNoise* perlinNoise2;
    PerlinNoise* perlinNoise3;
    PerlinNoise* mScaleNoise;
    PerlinNoise* mDepthNoise;
    PerlinNoise* mForestNoise;

    arrayWithLength<Biome*> biomes;
};

static BetaLevelSource* hBetaLevelSource;
