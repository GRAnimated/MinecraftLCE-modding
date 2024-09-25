#pragma once

#include <vector>
#include "Minecraft.Core/BlockPos.h"
#include "Minecraft.World/Random.h"
#include "Minecraft.World/level/levelgen/feature/Feature.h"
#include "feature/BetaTreeFeature.h"

class Block;

class BetaBasicTree : public BetaTreeFeature {
public:
    BetaBasicTree();

    void init(double heightScale, double widthScale, double foliageDensity);
    virtual bool place(Level* level, Random& random, const BlockPos& pos) override;

    void prepare();
    void crossSection(int x, int y, int z, float radius, int axis, Block* block);
    float treeShape(int y);
    float foliageShape(int y);
    void foliageCluster(int x, int y, int z);
    void limb(int paramArrayOfint1[], int paramArrayOfint2[], Block* block);
    void makeFoliage();
    bool trimBranches(int y);
    void makeTrunk();
    void makeBranches();
    int checkLine(int paramArrayOfint1[], int paramArrayOfint2[]);
    bool checkLocation();

private:
    static constexpr int axisConversionArray[6] = {2, 0, 0, 1, 2, 1};
    static constexpr int MAX_FOLIAGE_COORDS = 100;  // Adjust this value based on your needs

    Random rnd;
    Level* thisLevel;
    int origin[3];
    int height = 0;
    int trunkHeight;
    double trunkHeightScale = 0.618;
    double branchDensity = 1.0;
    double branchSlope = 0.381;
    double widthScale = 1.0;
    double foliageDensity = 1.0;
    int trunkWidth = 1;
    int heightVariance = 12;
    int foliageHeight = 4;
    std::vector<std::array<int, 4>> foliageCoords;
    int foliageCoordsLength = 0;
};