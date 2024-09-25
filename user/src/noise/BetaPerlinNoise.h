#pragma once

#include "Minecraft.World/ArrayWithLength.h"
#include "Minecraft.World/level/levelgen/synth/Synth.h"
#include "noise/BetaImprovedNoise.h"

class BetaPerlinNoise : public Synth {
public:
    BetaPerlinNoise(Random* random, int levels);
    virtual ~BetaPerlinNoise();

    virtual double getValue(double x, double y) override;
    double getValue(double x, double y, double z);

    arrayWithLength<double> getRegion(arrayWithLength<double> region, double x, double y, double z,
                                      int width, int height, int depth, double frequencyX,
                                      double frequencyY, double frequencyZ);

    arrayWithLength<double> getRegion(arrayWithLength<double> region, int paramInt1, int paramInt2,
                                      int paramInt3, int paramInt4, double paramDouble1,
                                      double paramDouble2, double paramDouble3);

private:
    BetaImprovedNoise** noiseLevels;
    int levels;
};
