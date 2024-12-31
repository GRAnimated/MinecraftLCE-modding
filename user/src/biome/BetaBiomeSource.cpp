#include "biome/BetaBiomeSource.h"
#include "Minecraft.World/ArrayWithLength.h"
#include "Minecraft.World/Random.h"
#include "Minecraft.World/level/Level.h"
#include "Minecraft.World/level/biome/Biome.h"
#include "Minecraft.World/level/levelgen/synth/PerlinSimplexNoise.h"
#include "biome/BetaBiome.h"
#include "biome/CustomBiomes.h"
#include "world/BetaLevelSource.h"

BetaBiomeSource::BetaBiomeSource(long long seed) {
    Random* temperatureRandom = new Random();
    Random* downfallRandom = new Random();
    Random* noiseRandom = new Random();

    temperatureRandom->setSeed(seed * 9871L);
    downfallRandom->setSeed(seed * 39811L);
    noiseRandom->setSeed(seed * 543321L);

    temperatureMap = new PerlinSimplexNoise(temperatureRandom, 4);
    downfallMap = new PerlinSimplexNoise(downfallRandom, 4);
    noiseMap = new PerlinSimplexNoise(noiseRandom, 2);

    temperatures = arrayWithLength<double>(256, true);
    downfalls = arrayWithLength<double>(256, true);
    noises = arrayWithLength<double>(256, true);
}

/*
Biome* BetaBiomeSource::getBiome(BlockPos paramChunkPos) {
    return getBiome(paramChunkPos.x * 16, paramChunkPos.z * 16);
}
*/

Biome* BetaBiomeSource::getBiome(int paramInt1, int paramInt2) {
    return getBiomeBlock(paramInt1, paramInt2, 1, 1)[0];
}

double BetaBiomeSource::getTemperature(int paramInt1, int paramInt2) {
    temperatures = temperatureMap->getRegion(temperatures, paramInt1, paramInt2, 1, 1,
                                             0.02500000037252903, 0.02500000037252903, 0.5);
    return temperatures[0];
}

double BetaBiomeSource::getDownfall(int paramInt1, int paramInt2) {
    downfalls = downfallMap->getRegion(downfalls, paramInt1, paramInt2, 1, 1, 0.05000000074505806,
                                       0.05000000074505806, 0.5);
    return downfalls[0];
}

arrayWithLength<Biome*> BetaBiomeSource::getBiomeBlock(int paramInt1, int paramInt2, int paramInt3,
                                                       int paramInt4) {
    biomes = getBiomeBlock(biomes, paramInt1, paramInt2, paramInt3, paramInt4);
    return biomes;
}

arrayWithLength<double>
BetaBiomeSource::getTemperatureBlock(arrayWithLength<double> paramArrayOfdouble, int paramInt1,
                                     int paramInt2, int paramInt3, int paramInt4) {
    if (paramArrayOfdouble.data == nullptr ||
        int(paramArrayOfdouble.length) < paramInt3 * paramInt4)
        paramArrayOfdouble = arrayWithLength<double>(paramInt3 * paramInt4, true);
    paramArrayOfdouble =
        temperatureMap->getRegion(paramArrayOfdouble, paramInt1, paramInt2, paramInt3, paramInt3,
                                  0.02500000037252903, 0.02500000037252903, 0.25);

    arrayWithLength<double> tempNoises = arrayWithLength<double>(paramInt3 * paramInt4, true);

    tempNoises = noiseMap->getRegion(tempNoises, paramInt1, paramInt2, paramInt3, paramInt3, 0.25,
                                     0.25, 0.5882352941176471);
    int b1 = 0;
    for (int b2 = 0; b2 < paramInt3; b2++) {
        for (int b = 0; b < paramInt4; b++) {
            double d1 = tempNoises[b1] * 1.1 + 0.5;
            double d2 = 0.01;
            double d3 = 1.0 - d2;
            double d4 = (paramArrayOfdouble[b1] * 0.15 + 0.7) * d3 + d1 * d2;
            d4 = 1.0 - (1.0 - d4) * (1.0 - d4);
            if (d4 < 0.0)
                d4 = 0.0;
            if (d4 > 1.0)
                d4 = 1.0;
            paramArrayOfdouble[b1] = d4;
            b1++;
        }
    }

    if (tempNoises.data) {
        delete[] reinterpret_cast<double*>(tempNoises.data);
    }

    return paramArrayOfdouble;
}

arrayWithLength<double>
BetaBiomeSource::getDownfallBlock(arrayWithLength<double> paramArrayOfdouble, int paramInt1,
                                  int paramInt2, int paramInt3, int paramInt4) {
    if (paramArrayOfdouble.data == nullptr ||
        int(paramArrayOfdouble.length) < paramInt3 * paramInt4)
        paramArrayOfdouble = arrayWithLength<double>(paramInt3 * paramInt4, true);
    return downfallMap->getRegion(paramArrayOfdouble, paramInt1, paramInt2, paramInt3, paramInt3,
                                  0.05000000074505806, 0.05000000074505806, 0.5);
}

arrayWithLength<Biome*> BetaBiomeSource::getBiomeBlock(arrayWithLength<Biome*> paramArrayOfBiome,
                                                       int paramInt1, int paramInt2, int paramInt3,
                                                       int paramInt4) {
    if (paramArrayOfBiome.data == nullptr || int(paramArrayOfBiome.length) < paramInt3 * paramInt4)
        paramArrayOfBiome = arrayWithLength<Biome*>(paramInt3 * paramInt4, true);
    temperatures =
        temperatureMap->getRegion(temperatures, paramInt1, paramInt2, paramInt3, paramInt3,
                                  0.02500000037252903, 0.02500000037252903, 0.25);
    downfalls =
        downfallMap->getRegion(downfalls, paramInt1, paramInt2, paramInt3, paramInt3,
                               0.05000000074505806, 0.05000000074505806, 0.3333333333333333);
    noises = noiseMap->getRegion(noises, paramInt1, paramInt2, paramInt3, paramInt3, 0.25, 0.25,
                                 0.5882352941176471);
    int b1 = 0;
    for (int b2 = 0; b2 < paramInt3; b2++) {
        for (int b = 0; b < paramInt4; b++) {
            double d1 = noises[b1] * 1.1 + 0.5;
            double d2 = 0.01;
            double d3 = 1.0 - d2;
            double d4 = (temperatures[b1] * 0.15 + 0.7) * d3 + d1 * d2;
            d2 = 0.002;
            d3 = 1.0 - d2;
            double d5 = (downfalls[b1] * 0.15 + 0.5) * d3 + d1 * d2;
            d4 = 1.0 - (1.0 - d4) * (1.0 - d4);
            if (d4 < 0.0)
                d4 = 0.0;
            if (d5 < 0.0)
                d5 = 0.0;
            if (d4 > 1.0)
                d4 = 1.0;
            if (d5 > 1.0)
                d5 = 1.0;
            temperatures[b1] = d4;
            // downfalls[b1] = d5;  // TODO: get downfalls working
            // temperatures[b1] = 1.0;
            downfalls[b1] = 0.96;
            paramArrayOfBiome[b1++] = BetaBiome::getBiome(d4, d5);
            // paramArrayOfBiome[b1++] = CustomBiomes::RAINFOREST;
        }
    }
    return paramArrayOfBiome;
}

arrayWithLength<Biome*>
BetaBiomeSource::nonDestructiveGetBiomeBlock(arrayWithLength<Biome*> paramArrayOfBiome,
                                             int paramInt1, int paramInt2, int paramInt3,
                                             int paramInt4) {
    if (paramArrayOfBiome.data == nullptr || int(paramArrayOfBiome.length) < paramInt3 * paramInt4)
        paramArrayOfBiome = arrayWithLength<Biome*>(paramInt3 * paramInt4, true);

    arrayWithLength<double> temps = arrayWithLength<double>();
    arrayWithLength<double> dfs = arrayWithLength<double>();
    arrayWithLength<double> ns = arrayWithLength<double>();

    temps = temperatureMap->getRegion(temps, paramInt1, paramInt2, paramInt3, paramInt3,
                                      0.02500000037252903, 0.02500000037252903, 0.25);
    dfs = downfallMap->getRegion(dfs, paramInt1, paramInt2, paramInt3, paramInt3,
                                 0.05000000074505806, 0.05000000074505806, 0.3333333333333333);
    ns = noiseMap->getRegion(ns, paramInt1, paramInt2, paramInt3, paramInt3, 0.25, 0.25,
                             0.5882352941176471);
    int b1 = 0;
    for (int b2 = 0; b2 < paramInt3; b2++) {
        for (int b = 0; b < paramInt4; b++) {
            double d1 = ns[b1] * 1.1 + 0.5;
            double d2 = 0.01;
            double d3 = 1.0 - d2;
            double d4 = (temps[b1] * 0.15 + 0.7) * d3 + d1 * d2;
            d2 = 0.002;
            d3 = 1.0 - d2;
            double d5 = (dfs[b1] * 0.15 + 0.5) * d3 + d1 * d2;
            d4 = 1.0 - (1.0 - d4) * (1.0 - d4);
            if (d4 < 0.0)
                d4 = 0.0;
            if (d5 < 0.0)
                d5 = 0.0;
            if (d4 > 1.0)
                d4 = 1.0;
            if (d5 > 1.0)
                d5 = 1.0;
            temps[b1] = d4;
            dfs[b1] = d5;
            paramArrayOfBiome[b1++] = BetaBiome::getBiome(d4, d5);
        }
    }

    if (temps.data) {
        delete[] reinterpret_cast<double*>(temps.data);
    }
    if (dfs.data) {
        delete[] reinterpret_cast<double*>(dfs.data);
    }
    if (ns.data) {
        delete[] reinterpret_cast<double*>(ns.data);
    }

    return paramArrayOfBiome;
}

double BetaBiomeSource::getTempBlock(int x, int z) {
    arrayWithLength<double> temps = arrayWithLength<double>();
    arrayWithLength<double> dfs = arrayWithLength<double>();
    arrayWithLength<double> ns = arrayWithLength<double>();

    temps = temperatureMap->getRegion(temps, x, z, 1, 1, 0.02500000037252903, 0.02500000037252903,
                                      0.25);
    dfs = downfallMap->getRegion(dfs, x, z, 1, 1, 0.05000000074505806, 0.05000000074505806,
                                 0.3333333333333333);
    ns = noiseMap->getRegion(ns, x, z, 1, 1, 0.25, 0.25, 0.5882352941176471);

    double temp = temps[0];
    double downfall = dfs[0];
    double noise = ns[0];

    if (temps.data) {
        delete[] reinterpret_cast<double*>(temps.data);
    }
    if (dfs.data) {
        delete[] reinterpret_cast<double*>(dfs.data);
    }
    if (ns.data) {
        delete[] reinterpret_cast<double*>(ns.data);
    }

    double d1 = noise * 1.1 + 0.5;
    double d2 = 0.01;
    double d3 = 1.0 - d2;
    double d4 = (temp * 0.15 + 0.7) * d3 + d1 * d2;
    d2 = 0.002;
    d3 = 1.0 - d2;
    double d5 = (downfall * 0.15 + 0.5) * d3 + d1 * d2;
    d4 = 1.0 - (1.0 - d4) * (1.0 - d4);
    if (d4 < 0.0)
        d4 = 0.0;
    if (d5 < 0.0)
        d5 = 0.0;
    if (d4 > 1.0)
        d4 = 1.0;
    if (d5 > 1.0)
        d5 = 1.0;
    temp = d4;
    downfall = d5;

    return temp;
}

double BetaBiomeSource::getDownBlock(int x, int z) {
    arrayWithLength<double> temps = arrayWithLength<double>();
    arrayWithLength<double> dfs = arrayWithLength<double>();
    arrayWithLength<double> ns = arrayWithLength<double>();

    temps = temperatureMap->getRegion(temps, x, z, 1, 1, 0.02500000037252903, 0.02500000037252903,
                                      0.25);
    dfs = downfallMap->getRegion(dfs, x, z, 1, 1, 0.05000000074505806, 0.05000000074505806,
                                 0.3333333333333333);
    ns = noiseMap->getRegion(ns, x, z, 1, 1, 0.25, 0.25, 0.5882352941176471);

    double temp = temps[0];
    double downfall = dfs[0];
    double noise = ns[0];

    if (temps.data) {
        delete[] reinterpret_cast<double*>(temps.data);
    }
    if (dfs.data) {
        delete[] reinterpret_cast<double*>(dfs.data);
    }
    if (ns.data) {
        delete[] reinterpret_cast<double*>(ns.data);
    }

    double d1 = noise * 1.1 + 0.5;
    double d2 = 0.01;
    double d3 = 1.0 - d2;
    double d4 = (temp * 0.15 + 0.7) * d3 + d1 * d2;
    d2 = 0.002;
    d3 = 1.0 - d2;
    double d5 = (downfall * 0.15 + 0.5) * d3 + d1 * d2;
    d4 = 1.0 - (1.0 - d4) * (1.0 - d4);
    if (d4 < 0.0)
        d4 = 0.0;
    if (d5 < 0.0)
        d5 = 0.0;
    if (d4 > 1.0)
        d4 = 1.0;
    if (d5 > 1.0)
        d5 = 1.0;
    temp = d4;
    downfall = d5;

    return downfall;
}