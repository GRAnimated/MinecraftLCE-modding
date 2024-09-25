#include "noise/BetaPerlinNoise.h"
#include "Minecraft.World/ArrayWithLength.h"

BetaPerlinNoise::BetaPerlinNoise(Random* paramRandom, int paramInt) {
    levels = paramInt;
    noiseLevels = new BetaImprovedNoise*[paramInt];
    for (int b = 0; b < paramInt; b++)
        noiseLevels[b] = new BetaImprovedNoise(paramRandom);
}

BetaPerlinNoise::~BetaPerlinNoise() {
    for (int b = 0; b < levels; b++)
        delete noiseLevels[b];
    delete[] noiseLevels;
}

double BetaPerlinNoise::getValue(double paramDouble1, double paramDouble2) {
    double d1 = 0.0;
    double d2 = 1.0;
    for (int b = 0; b < levels; b++) {
        d1 += noiseLevels[b]->getValue(paramDouble1 * d2, paramDouble2 * d2) / d2;
        d2 /= 2.0;
    }
    return d1;
}

double BetaPerlinNoise::getValue(double paramDouble1, double paramDouble2, double paramDouble3) {
    double d1 = 0.0;
    double d2 = 1.0;
    for (int b = 0; b < levels; b++) {
        d1 +=
            noiseLevels[b]->getValue(paramDouble1 * d2, paramDouble2 * d2, paramDouble3 * d2) / d2;
        d2 /= 2.0;
    }
    return d1;
}

arrayWithLength<double> BetaPerlinNoise::getRegion(arrayWithLength<double> paramArrayOfdouble,
                                                   double paramDouble1, double paramDouble2,
                                                   double paramDouble3, int paramInt1,
                                                   int paramInt2, int paramInt3,
                                                   double paramDouble4, double paramDouble5,
                                                   double paramDouble6) {
    if (paramArrayOfdouble.data == nullptr) {
        paramArrayOfdouble = arrayWithLength<double>(paramInt1 * paramInt2 * paramInt3, true);
    } else {
        for (unsigned int b1 = 0; b1 < paramArrayOfdouble.length; b1++)
            paramArrayOfdouble[b1] = 0.0;
    }
    double d = 1.0;
    for (int b = 0; b < levels; b++) {
        noiseLevels[b]->add(paramArrayOfdouble, paramDouble1, paramDouble2, paramDouble3, paramInt1,
                            paramInt2, paramInt3, paramDouble4 * d, paramDouble5 * d,
                            paramDouble6 * d, d);
        d /= 2.0;
    }
    return paramArrayOfdouble;
}

arrayWithLength<double> BetaPerlinNoise::getRegion(arrayWithLength<double> paramArrayOfdouble,
                                                   int paramInt1, int paramInt2, int paramInt3,
                                                   int paramInt4, double paramDouble1,
                                                   double paramDouble2, double paramDouble3) {
    return getRegion(paramArrayOfdouble, paramInt1, 10.0, paramInt2, paramInt3, 1, paramInt4,
                     paramDouble1, 1.0, paramDouble2);
}
