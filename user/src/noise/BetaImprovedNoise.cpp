#include "noise/BetaImprovedNoise.h"
#include "Minecraft.World/Random.h"

BetaImprovedNoise::BetaImprovedNoise() : BetaImprovedNoise(new Random()) {}

BetaImprovedNoise::BetaImprovedNoise(Random* paramRandom) {
    xo = paramRandom->nextDouble() * 256.0;
    yo = paramRandom->nextDouble() * 256.0;
    zo = paramRandom->nextDouble() * 256.0;
    for (int b = 0; b < 256; b++)
        p[b] = b;
    for (int b = 0; b < 256; b++) {
        int i = paramRandom->nextInt(256 - b) + b;
        int j = p[b];
        p[b] = p[i];
        p[i] = j;
        p[b + 256] = p[b];
    }
}

double BetaImprovedNoise::noise(double paramDouble1, double paramDouble2, double paramDouble3) {
    double d1 = paramDouble1 + xo;
    double d2 = paramDouble2 + yo;
    double d3 = paramDouble3 + zo;
    int i = (int)d1;
    int j = (int)d2;
    int k = (int)d3;
    if (d1 < i)
        i--;
    if (d2 < j)
        j--;
    if (d3 < k)
        k--;
    int m = i & 0xFF;
    int n = j & 0xFF;
    int i1 = k & 0xFF;
    d1 -= i;
    d2 -= j;
    d3 -= k;
    double d4 = d1 * d1 * d1 * (d1 * (d1 * 6.0 - 15.0) + 10.0);
    double d5 = d2 * d2 * d2 * (d2 * (d2 * 6.0 - 15.0) + 10.0);
    double d6 = d3 * d3 * d3 * (d3 * (d3 * 6.0 - 15.0) + 10.0);
    int i2 = p[m] + n;
    int i3 = p[i2] + i1;
    int i4 = p[i2 + 1] + i1;
    int i5 = p[m + 1] + n;
    int i6 = p[i5] + i1;
    int i7 = p[i5 + 1] + i1;
    return lerp(
        d6,
        lerp(d5, lerp(d4, grad(p[i3], d1, d2, d3), grad(p[i6], d1 - 1.0, d2, d3)),
             lerp(d4, grad(p[i4], d1, d2 - 1.0, d3), grad(p[i7], d1 - 1.0, d2 - 1.0, d3))),
        lerp(d5,
             lerp(d4, grad(p[i3 + 1], d1, d2, d3 - 1.0), grad(p[i6 + 1], d1 - 1.0, d2, d3 - 1.0)),
             lerp(d4, grad(p[i4 + 1], d1, d2 - 1.0, d3 - 1.0),
                  grad(p[i7 + 1], d1 - 1.0, d2 - 1.0, d3 - 1.0))));
}

double BetaImprovedNoise::lerp(double paramDouble1, double paramDouble2, double paramDouble3) {
    return paramDouble2 + paramDouble1 * (paramDouble3 - paramDouble2);
}

double BetaImprovedNoise::grad2(int paramInt, double paramDouble1, double paramDouble2) {
    int i = paramInt & 0xF;
    double d1 = (1 - ((i & 0x8) >> 3)) * paramDouble1;
    double d2 = (i < 4) ? 0.0 : ((i == 12 || i == 14) ? paramDouble1 : paramDouble2);
    return (((i & 0x1) == 0) ? d1 : -d1) + (((i & 0x2) == 0) ? d2 : -d2);
}

double BetaImprovedNoise::grad(int paramInt, double paramDouble1, double paramDouble2,
                               double paramDouble3) {
    int i = paramInt & 0xF;
    double d1 = (i < 8) ? paramDouble1 : paramDouble2;
    double d2 = (i < 4) ? paramDouble2 : ((i == 12 || i == 14) ? paramDouble1 : paramDouble3);
    return (((i & 0x1) == 0) ? d1 : -d1) + (((i & 0x2) == 0) ? d2 : -d2);
}

double BetaImprovedNoise::getValue(double paramDouble1, double paramDouble2) {
    return noise(paramDouble1, paramDouble2, 0.0);
}

double BetaImprovedNoise::getValue(double paramDouble1, double paramDouble2, double paramDouble3) {
    return noise(paramDouble1, paramDouble2, paramDouble3);
}

void BetaImprovedNoise::add(arrayWithLength<double> paramArrayOfdouble, double paramDouble1,
                            double paramDouble2, double paramDouble3, int paramInt1, int paramInt2,
                            int paramInt3, double paramDouble4, double paramDouble5,
                            double paramDouble6, double paramDouble7) {
    if (paramInt2 == 1) {
        int i3 = 0;
        int i4 = 0;
        int i5 = 0;
        int i6 = 0;
        double d6 = 0.0;
        double d7 = 0.0;
        int b3 = 0;
        double d8 = 1.0 / paramDouble7;
        for (int b4 = 0; b4 < paramInt1; b4++) {
            double d9 = (paramDouble1 + b4) * paramDouble4 + xo;
            int i7 = (int)d9;
            if (d9 < i7)
                i7--;
            int i8 = i7 & 0xFF;
            d9 -= i7;
            double d10 = d9 * d9 * d9 * (d9 * (d9 * 6.0 - 15.0) + 10.0);
            for (int b = 0; b < paramInt3; b++) {
                double d11 = (paramDouble3 + b) * paramDouble6 + zo;
                int i9 = (int)d11;
                if (d11 < i9)
                    i9--;
                int i10 = i9 & 0xFF;
                d11 -= i9;
                double d12 = d11 * d11 * d11 * (d11 * (d11 * 6.0 - 15.0) + 10.0);
                i3 = p[i8] + 0;
                i4 = p[i3] + i10;
                i5 = p[i8 + 1] + 0;
                i6 = p[i5] + i10;
                d6 = lerp(d10, grad2(p[i4], d9, d11), grad(p[i6], d9 - 1.0, 0.0, d11));
                d7 = lerp(d10, grad(p[i4 + 1], d9, 0.0, d11 - 1.0),
                          grad(p[i6 + 1], d9 - 1.0, 0.0, d11 - 1.0));
                double d13 = lerp(d12, d6, d7);
                paramArrayOfdouble[b3++] = paramArrayOfdouble[b3++] + d13 * d8;
            }
        }
        return;
    }
    int b1 = 0;
    double d1 = 1.0 / paramDouble7;
    int i = -1;
    int j = 0;
    int k = 0;
    int m = 0;
    int n = 0;
    int i1 = 0;
    int i2 = 0;
    double d2 = 0.0;
    double d3 = 0.0;
    double d4 = 0.0;
    double d5 = 0.0;
    for (int b2 = 0; b2 < paramInt1; b2++) {
        double d6 = (paramDouble1 + b2) * paramDouble4 + xo;
        int i3 = (int)d6;
        if (d6 < i3)
            i3--;
        int i4 = i3 & 0xFF;
        d6 -= i3;
        double d7 = d6 * d6 * d6 * (d6 * (d6 * 6.0 - 15.0) + 10.0);
        for (int b = 0; b < paramInt3; b++) {
            double d8 = (paramDouble3 + b) * paramDouble6 + zo;
            int i5 = (int)d8;
            if (d8 < i5)
                i5--;
            int i6 = i5 & 0xFF;
            d8 -= i5;
            double d9 = d8 * d8 * d8 * (d8 * (d8 * 6.0 - 15.0) + 10.0);
            for (int b3 = 0; b3 < paramInt2; b3++) {
                double d10 = (paramDouble2 + b3) * paramDouble5 + yo;
                int i7 = (int)d10;
                if (d10 < i7)
                    i7--;
                int i8 = i7 & 0xFF;
                d10 -= i7;
                double d11 = d10 * d10 * d10 * (d10 * (d10 * 6.0 - 15.0) + 10.0);
                if (b3 == 0 || i8 != i) {
                    i = i8;
                    j = p[i4] + i8;
                    k = p[j] + i6;
                    m = p[j + 1] + i6;
                    n = p[i4 + 1] + i8;
                    i1 = p[n] + i6;
                    i2 = p[n + 1] + i6;
                    d2 = lerp(d7, grad(p[k], d6, d10, d8), grad(p[i1], d6 - 1.0, d10, d8));
                    d3 = lerp(d7, grad(p[m], d6, d10 - 1.0, d8),
                              grad(p[i2], d6 - 1.0, d10 - 1.0, d8));
                    d4 = lerp(d7, grad(p[k + 1], d6, d10, d8 - 1.0),
                              grad(p[i1 + 1], d6 - 1.0, d10, d8 - 1.0));
                    d5 = lerp(d7, grad(p[m + 1], d6, d10 - 1.0, d8 - 1.0),
                              grad(p[i2 + 1], d6 - 1.0, d10 - 1.0, d8 - 1.0));
                }
                double d12 = lerp(d11, d2, d3);
                double d13 = lerp(d11, d4, d5);
                double d14 = lerp(d9, d12, d13);
                paramArrayOfdouble[b1++] = paramArrayOfdouble[b1++] + d14 * d1;
            }
        }
    }
}
