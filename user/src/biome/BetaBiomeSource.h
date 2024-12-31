#pragma once

#include "Minecraft.World/ArrayWithLength.h"

class PerlinSimplexNoise;
class Biome;
class Level;
class BlockPos;

class BetaBiomeSource {
public:
    static void initBiomes();

    PerlinSimplexNoise* temperatureMap;
    PerlinSimplexNoise* downfallMap;
    PerlinSimplexNoise* noiseMap;

    arrayWithLength<double> temperatures;
    arrayWithLength<double> downfalls;
    arrayWithLength<double> noises;
    arrayWithLength<Biome*> biomes;

    BetaBiomeSource(long long seed);

    // Biome* getBiome(BlockPos paramChunkPos);

    Biome* getBiome(int paramInt1, int paramInt2);
    double getTemperature(int paramInt1, int paramInt2);
    double getDownfall(int paramInt1, int paramInt2);

    arrayWithLength<Biome*> getBiomeBlock(int paramInt1, int paramInt2, int paramInt3,
                                          int paramInt4);
    arrayWithLength<double> getTemperatureBlock(arrayWithLength<double> paramArrayOfdouble,
                                                int paramInt1, int paramInt2, int paramInt3,
                                                int paramInt4);
    arrayWithLength<double> getDownfallBlock(arrayWithLength<double> paramArrayOfdouble,
                                             int paramInt1, int paramInt2, int paramInt3,
                                             int paramInt4);
    arrayWithLength<Biome*> getBiomeBlock(arrayWithLength<Biome*> paramArrayOfBiome, int paramInt1,
                                          int paramInt2, int paramInt3, int paramInt4);
    arrayWithLength<Biome*> nonDestructiveGetBiomeBlock(arrayWithLength<Biome*> paramArrayOfBiome,
                                                        int paramInt1, int paramInt2, int paramInt3,
                                                        int paramInt4);

    double getTempBlock(int x, int z);
    double getDownBlock(int x, int z);
};
