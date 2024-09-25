#include "feature/BetaBasicTree.h"
#include "Minecraft.World/level/Level.h"
#include "Minecraft.World/level/block/Blocks.h"

BetaBasicTree::BetaBasicTree() : BetaTreeFeature() {
    init(1.0, 1.0, 1.0);
}

void BetaBasicTree::init(double paramDouble1, double paramDouble2, double paramDouble3) {
    heightVariance = (int)(paramDouble1 * 12.0);
    if (paramDouble1 > 0.5)
        foliageHeight = 5;
    widthScale = paramDouble2;
    foliageDensity = paramDouble3;
}

bool BetaBasicTree::place(Level* level, Random& random, const BlockPos& pos) {
    thisLevel = level;
    rnd.setSeed(random.nextLong());
    origin[0] = pos.x;
    origin[1] = pos.y;
    origin[2] = pos.z;
    if (height == 0)
        height = 5 + rnd.nextInt(heightVariance);
    if (!checkLocation())
        return false;
    thisLevel->setBlock(pos, Blocks::DIRT->defaultBlockState(), 2, false);
    prepare();
    makeFoliage();
    makeTrunk();
    makeBranches();
    return true;
}

void BetaBasicTree::prepare() {
    trunkHeight = (int)(height * trunkHeightScale);
    if (trunkHeight >= height)
        trunkHeight = height - 1;
    int i = (int)(1.382 + std::pow(foliageDensity * height / 13.0, 2.0));
    if (i < 1)
        i = 1;
    std::vector<std::array<int, 4>> arrayOfInt(i * height);
    int j = origin[1] + height - foliageHeight;
    int b = 1;
    int k = origin[1] + trunkHeight;
    int m = j - origin[1];
    arrayOfInt[0][0] = origin[0];
    arrayOfInt[0][1] = j;
    arrayOfInt[0][2] = origin[2];
    arrayOfInt[0][3] = k;
    j--;
    while (m >= 0) {
        int b1 = 0;
        float f = treeShape(m);
        if (f < 0.0F) {
            j--;
            m--;
            continue;
        }
        double d = 0.5;
        while (b1 < i) {
            double d1 = widthScale * f * (rnd.nextFloat() + 0.328);
            double d2 = rnd.nextFloat() * 2.0 * 3.14159;
            int n = (int)(d1 * std::sin(d2) + origin[0] + d);
            int i1 = (int)(d1 * std::cos(d2) + origin[2] + d);
            int arrayOfInt1[] = {n, j, i1};
            int arrayOfInt2[] = {n, j + foliageHeight, i1};
            if (checkLine(arrayOfInt1, arrayOfInt2) == -1) {
                int arrayOfInt3[] = {origin[0], origin[1], origin[2]};
                double d3 = std::sqrt(std::pow(std::abs(origin[0] - arrayOfInt1[0]), 2.0) +
                                      std::pow(std::abs(origin[2] - arrayOfInt1[2]), 2.0));
                double d4 = d3 * branchSlope;
                if (arrayOfInt1[1] - d4 > k) {
                    arrayOfInt3[1] = k;
                } else {
                    arrayOfInt3[1] = (int)(arrayOfInt1[1] - d4);
                }
                if (checkLine(arrayOfInt3, arrayOfInt1) == -1) {
                    arrayOfInt[b][0] = n;
                    arrayOfInt[b][1] = j;
                    arrayOfInt[b][2] = i1;
                    arrayOfInt[b][3] = arrayOfInt3[1];
                    b++;
                }
            }
            b1++;
        }
        j--;
        m--;
    }

    foliageCoords.resize(b);

    for (int i = 0; i < b; i++) {
        foliageCoords[i] = arrayOfInt[i];
    }
}

void BetaBasicTree::crossSection(int x, int y, int z, float radius, int axis, Block* block) {
    int maxRadius = int(radius + 0.618);
    int axis1 = axisConversionArray[axis];
    int axis2 = axisConversionArray[axis + 3];
    int center[3] = {x, y, z};
    int pos[3] = {0, 0, 0};

    for (int dx = -maxRadius; dx <= maxRadius; dx++) {
        pos[axis] = center[axis];
        for (int dz = -maxRadius; dz <= maxRadius; dz++) {
            double distance =
                std::sqrt(std::pow(std::abs(dx) + 0.5, 2.0) + std::pow(std::abs(dz) + 0.5, 2.0));
            if (distance > radius)
                continue;

            pos[axis1] = center[axis1] + dx;
            pos[axis2] = center[axis2] + dz;

            if (thisLevel->getBlock(BlockPos(pos[0], pos[1], pos[2])) == Blocks::AIR ||
                thisLevel->getBlock(BlockPos(pos[0], pos[1], pos[2])) == Blocks::LEAVES) {
                thisLevel->setBlock(BlockPos(pos[0], pos[1], pos[2]), block->defaultBlockState(), 2,
                                    false);
            }
        }
    }
}

float BetaBasicTree::treeShape(int paramInt) {
    float f3;
    if (paramInt < height * 0.3)
        return -1.618;
    float f1 = height / 2.0;
    float f2 = height / 2.0 - paramInt;
    if (f2 == 0.0F) {
        f3 = f1;
    } else if (std::abs(f2) >= f1) {
        f3 = 0.0F;
    } else {
        f3 = (float)std::sqrt(std::pow(std::abs(f1), 2.0) - std::pow(std::abs(f2), 2.0));
    }
    f3 *= 0.5F;
    return f3;
}

float BetaBasicTree::foliageShape(int y) {
    if (y < 0 || y >= foliageHeight) {
        return -1.0f;
    }
    return (y == 0 || y == foliageHeight - 1) ? 2.0f : 3.0f;
}

void BetaBasicTree::foliageCluster(int x, int y, int z) {
    for (int i = y; i < y + foliageHeight; i++) {
        float f = foliageShape(i - y);
        crossSection(x, i, z, f, 1, Blocks::LEAVES);
    }
}

void BetaBasicTree::limb(int paramArrayOfint1[], int paramArrayOfint2[], Block* paramInt) {
    int b5;
    int arrayOfInt1[] = {0, 0, 0};
    int b1 = 0;
    int b2 = 0;
    while (b1 < 3) {
        arrayOfInt1[b1] = paramArrayOfint2[b1] - paramArrayOfint1[b1];
        if (std::abs(arrayOfInt1[b1]) > std::abs(arrayOfInt1[b2]))
            b2 = b1;
        b1 = (b1 + 1);
    }
    if (arrayOfInt1[b2] == 0)
        return;
    int b3 = axisConversionArray[b2];
    int b4 = axisConversionArray[b2 + 3];
    if (arrayOfInt1[b2] > 0) {
        b5 = 1;
    } else {
        b5 = -1;
    }
    double d1 = double(arrayOfInt1[b3]) / arrayOfInt1[b2];
    double d2 = double(arrayOfInt1[b4]) / arrayOfInt1[b2];
    int arrayOfInt2[] = {0, 0, 0};
    int i = 0;
    int j = arrayOfInt1[b2] + b5;
    while (i != j) {
        arrayOfInt2[b2] = std::floor((paramArrayOfint1[b2] + i) + 0.5);
        arrayOfInt2[b3] = std::floor(paramArrayOfint1[b3] + i * d1 + 0.5);
        arrayOfInt2[b4] = std::floor(paramArrayOfint1[b4] + i * d2 + 0.5);
        thisLevel->setBlock(BlockPos(arrayOfInt2[0], arrayOfInt2[1], arrayOfInt2[2]),
                            paramInt->defaultBlockState(), 2, false);
        i += b5;
    }
}

void BetaBasicTree::makeFoliage() {
    for (size_t i = 0; i < foliageCoords.size(); i++) {
        foliageCluster(foliageCoords[i][0], foliageCoords[i][1], foliageCoords[i][2]);
    }
}

bool BetaBasicTree::trimBranches(int paramInt) {
    return !(paramInt < height * 0.2);
}

void BetaBasicTree::makeTrunk() {
    int i = origin[0];
    int j = origin[1];
    int k = origin[1] + trunkHeight;
    int m = origin[2];
    int arrayOfInt1[] = {i, j, m};
    int arrayOfInt2[] = {i, k, m};
    limb(arrayOfInt1, arrayOfInt2, Blocks::LOG);
    if (trunkWidth == 2) {
        arrayOfInt1[0] = arrayOfInt1[0] + 1;
        arrayOfInt2[0] = arrayOfInt2[0] + 1;
        limb(arrayOfInt1, arrayOfInt2, Blocks::LOG);
        arrayOfInt1[2] = arrayOfInt1[2] + 1;
        arrayOfInt2[2] = arrayOfInt2[2] + 1;
        limb(arrayOfInt1, arrayOfInt2, Blocks::LOG);
        arrayOfInt1[0] = arrayOfInt1[0] + -1;
        arrayOfInt2[0] = arrayOfInt2[0] + -1;
        limb(arrayOfInt1, arrayOfInt2, Blocks::LOG);
    }
}

void BetaBasicTree::makeBranches() {
    int b = 0;
    int i = foliageCoords.size();
    int arrayOfInt[] = {origin[0], origin[1], origin[2]};
    while (b < i) {
        int* arrayOfInt1 = foliageCoords[b].data();
        int arrayOfInt2[] = {arrayOfInt1[0], arrayOfInt1[1], arrayOfInt1[2]};
        arrayOfInt[1] = arrayOfInt1[3];
        int j = arrayOfInt[1] - origin[1];
        if (trimBranches(j))
            limb(arrayOfInt, arrayOfInt2, Blocks::LOG);
        b++;
    }
}

int BetaBasicTree::checkLine(int paramArrayOfint1[], int paramArrayOfint2[]) {
    int b5;
    int arrayOfInt1[] = {0, 0, 0};
    int b1 = 0;
    int b2 = 0;
    while (b1 < 3) {
        arrayOfInt1[b1] = paramArrayOfint2[b1] - paramArrayOfint1[b1];
        if (std::abs(arrayOfInt1[b1]) > std::abs(arrayOfInt1[b2]))
            b2 = b1;
        b1 = (b1 + 1);
    }
    if (arrayOfInt1[b2] == 0)
        return -1;
    int b3 = axisConversionArray[b2];
    int b4 = axisConversionArray[b2 + 3];
    if (arrayOfInt1[b2] > 0) {
        b5 = 1;
    } else {
        b5 = -1;
    }
    double d1 = double(arrayOfInt1[b3]) / arrayOfInt1[b2];
    double d2 = double(arrayOfInt1[b4]) / arrayOfInt1[b2];
    int arrayOfInt2[] = {0, 0, 0};
    int i = 0;
    int j = arrayOfInt1[b2] + b5;
    while (i != j) {
        arrayOfInt2[b2] = paramArrayOfint1[b2] + i;
        arrayOfInt2[b3] = (int)(paramArrayOfint1[b3] + i * d1);
        arrayOfInt2[b4] = (int)(paramArrayOfint1[b4] + i * d2);
        Block* k = thisLevel->getBlock(BlockPos(arrayOfInt2[0], arrayOfInt2[1], arrayOfInt2[2]));
        if (k != Blocks::AIR && k != Blocks::LEAVES)
            break;
        i += b5;
    }
    return (i == j) ? -1 : std::abs(i);
}

bool BetaBasicTree::checkLocation() {
    int arrayOfInt1[] = {origin[0], origin[1], origin[2]};
    int arrayOfInt2[] = {origin[0], origin[1] + height - 1, origin[2]};
    Block* i = thisLevel->getBlock(BlockPos(origin[0], origin[1] - 1, origin[2]));
    if (i != Blocks::GRASS && i != Blocks::DIRT)
        return false;
    int j = checkLine(arrayOfInt1, arrayOfInt2);
    if (j == -1)
        return true;
    if (j < 6)
        return false;
    height = j;
    return true;
}
