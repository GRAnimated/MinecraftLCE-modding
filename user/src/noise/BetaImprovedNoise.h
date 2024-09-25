#pragma once

#include "Minecraft.World/ArrayWithLength.h"
#include "Minecraft.World/level/levelgen/synth/Synth.h"

class Random;

class BetaImprovedNoise : public Synth {
public:
    BetaImprovedNoise();
    BetaImprovedNoise(Random* paramRandom);

    double noise(double paramDouble1, double paramDouble2, double paramDouble3);
    virtual double getValue(double paramDouble1, double paramDouble2) override;
    double getValue(double paramDouble1, double paramDouble2, double paramDouble3);

    void add(arrayWithLength<double> paramArrayOfdouble, double paramDouble1, double paramDouble2,
             double paramDouble3, int paramInt1, int paramInt2, int paramInt3, double paramDouble4,
             double paramDouble5, double paramDouble6, double paramDouble7);

private:
    int p[512];
    double scale;
    double xo;
    double yo;
    double zo;

    double lerp(double t, double a, double b);
    double grad2(int hash, double x, double y);
    double grad(int hash, double x, double y, double z);
};
