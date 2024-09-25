#include "biome/BetaBiome.h"
#include <algorithm>

#include "Minecraft.Client/Minecraft.h"
#include "Minecraft.Core/BlockPos.h"
#include "Minecraft.Util/Mth.h"
#include "Minecraft.World/ArrayWithLength.h"
#include "Minecraft.World/entity/Entity.h"
#include "Minecraft.World/level/biome/Biome.h"
#include "Minecraft.World/level/block/FoliageColor.h"
#include "Minecraft.World/level/block/GrassColor.h"
#include "Minecraft.World/level/storage/LevelSource.h"
#include "Minecraft.World/phys/Vec3.h"
#include "biome/BetaBiomeDecorator.h"
#include "biome/CustomBiomes.h"
#include "feature/BetaTreeFeature.h"
#include "mallow/hook/helpers.hpp"
#include "world/CustomLevelSource.h"

/*
template <typename T>
void align_vector_capacity(std::vector<T>& vec) {
    size_t current_size = vec.size();
    size_t current_capacity = vec.capacity();

    size_t aligned_capacity = current_capacity & ~7;

    if (aligned_capacity < current_size) {
        aligned_capacity = current_size;
    }

    vec.reserve(aligned_capacity);
}
*/

Biome* BetaBiome::map[4096] = {};

void BetaBiome::recalc() {
    for (int b = 0; b < 64; b++) {
        for (int b1 = 0; b1 < 64; b1++)
            map[b + b1 * 64] = _getBiome(b / 63.0F, b1 / 63.0F);
    }
}

Biome* BetaBiome::getBiome(double paramDouble1, double paramDouble2) {
    int i = (int)(paramDouble1 * 63.0);
    int j = (int)(paramDouble2 * 63.0);
    return map[i + j * 64];
}

Biome* BetaBiome::_getBiome(float temperature, float downfall) {
    downfall *= temperature;

    if (temperature < 0.1) {
        return CustomBiomes::TUNDRA;
    }

    if (downfall < 0.2) {
        if (temperature < 0.5) {
            return CustomBiomes::TUNDRA;
        } else if (temperature < 0.95) {
            return CustomBiomes::SAVANNA;
        } else {
            return CustomBiomes::DESERT;
        }
    }

    if (downfall > 0.5 && temperature < 0.7) {
        return CustomBiomes::SWAMPLAND;
    }

    if (temperature < 0.5) {
        return CustomBiomes::TAIGA;
    } else if (temperature < 0.97) {
        if (downfall < 0.35) {
            return CustomBiomes::SHRUBLAND;
        } else {
            return CustomBiomes::FOREST;
        }
    } else {
        if (downfall < 0.45) {
            return CustomBiomes::PLAINS;
        } else if (downfall < 0.9) {
            return CustomBiomes::SEASONAL_FOREST;
        } else {
            return CustomBiomes::RAINFOREST;
        }
    }
}

BetaBiome::BetaBiome(Biome::EBiomeIDs id, Biome::BiomeProperties* properties)
    : Biome(id, properties) {
    // align_vector_capacity(field_90);
    // align_vector_capacity(field_c0);
    // align_vector_capacity(field_d8);

    mBiomeDecorator = new BetaBiomeDecorator();
}

int BetaBiome::getBaseClass() {
    return 11;
}

uint BetaBiome::getGrassColor(BlockPos const* pos) {
    // int x = 0;
    // int z = 0;
    //
    // if (pos) {
    //     x = pos->x;
    //     z = pos->z;
    // }
    // double temperature = hCustomLevelSource->mBiomeSource->getTempBlock(x, z);
    // double downfall = hCustomLevelSource->mBiomeSource->getDownBlock(x, z);

    double temperature = 0.0;
    double downfall = 0.0;

    return GrassColor::get(temperature, downfall);

    //
    // hCustomLevelSource->mBiomeSource->getBiomeBlock(x, z, 1, 1);
    // double temperature = hCustomLevelSource->mBiomeSource->temperatures[0];
    // double downfall = hCustomLevelSource->mBiomeSource->downfalls[0];
    //
    // return GrassColor::get(temperature, downfall);

    // double temperature = 1.0;
    // double downfall = 1.0;
    // return GrassColor::get(temperature, downfall);
}

uint BetaBiome::getFoliageColor(BlockPos const* pos) {
    // int x = 0;
    // int z = 0;
    //
    // if (pos) {
    //     x = pos->x;
    //     z = pos->z;
    // }
    // double temperature = hCustomLevelSource->mBiomeSource->getTempBlock(x, z);
    // double downfall = hCustomLevelSource->mBiomeSource->getDownBlock(x, z);

    double temperature = 0.0;
    double downfall = 0.0;

    return FoliageColor::get(temperature, downfall);

    // //
    // // hCustomLevelSource->mBiomeSource->getBiomeBlock(x, z, 1, 1);
    // // double temperature = hCustomLevelSource->mBiomeSource->temperatures[0];
    // // double downfall = hCustomLevelSource->mBiomeSource->downfalls[0];
    // //
    // // return GrassColor::get(temperature, downfall);

    // double temperature = 1.0;
    // double downfall = 1.0;
    // return FoliageColor::get(temperature, downfall);
}

void BetaBiome::setGrassColor(BlockPos const* pos, uint color) {}

void BetaBiome::setFoliageColor(BlockPos const* pos, uint color) {}

void BetaBiome::decorate(Level* level, Random& random, const BlockPos* pos) {
    mBiomeDecorator->decorate(level, random, this, *pos);
}

struct BiomeGetTreeFeatureHook : mallow::hook::Trampoline<BiomeGetTreeFeatureHook> {
    static Feature* Callback(Biome* biome, Random& random) { return new BetaTreeFeature(); }
};

struct BiomeGetSkyColorHook : mallow::hook::Trampoline<BiomeGetSkyColorHook> {
    static uint Callback(Biome* biome, float time) {
        Entity* player = (Entity*)Minecraft::GetInstance()->mLocalPlayer;
        Vec3 pos = player->getPos(0.0f);

        double temp = CustomLevelSource::getInstance()->mBiomeSource->getTempBlock(pos.x, pos.z);

        temp /= 3.0;
        temp = std::clamp(temp, -1.0, 1.0);

        return Mth::hsvToRgb(0.6222222 - temp * 0.05, 0.5 + temp * 0.1, 1.0);
    }
};

struct BiomeColorsGetAverageGrassColorHook
    : mallow::hook::Trampoline<BiomeColorsGetAverageGrassColorHook> {
    static uint Callback(LevelSource* levelSource, BlockPos const& pos) {
        double temperature =
            CustomLevelSource::getInstance()->mBiomeSource->getTempBlock(pos.x, pos.z);
        double downfall =
            CustomLevelSource::getInstance()->mBiomeSource->getDownBlock(pos.x, pos.z);

        return GrassColor::get(temperature, downfall);
    }
};

struct BiomeColorsGetAverageFoliageColorHook
    : mallow::hook::Trampoline<BiomeColorsGetAverageFoliageColorHook> {
    static uint Callback(LevelSource* levelSource, BlockPos const& pos) {
        double temperature =
            CustomLevelSource::getInstance()->mBiomeSource->getTempBlock(pos.x, pos.z);
        double downfall =
            CustomLevelSource::getInstance()->mBiomeSource->getDownBlock(pos.x, pos.z);

        return FoliageColor::get(temperature, downfall);
    }
};

struct BiomeColorsGetAverageSkyColorHook
    : mallow::hook::Trampoline<BiomeColorsGetAverageSkyColorHook> {
    static uint Callback(LevelSource* levelSource, BlockPos const& pos) {
        double temperature =
            CustomLevelSource::getInstance()->mBiomeSource->getTempBlock(pos.x, pos.z);
        double downfall =
            CustomLevelSource::getInstance()->mBiomeSource->getDownBlock(pos.x, pos.z);

        Biome* biome = BetaBiome::getBiome(temperature, downfall);

        return biome->getSkyColor(0.0);
    }
};

void BetaBiome::initHooks() {
    BiomeGetTreeFeatureHook::InstallAtOffset(0x426AC);
    BiomeGetSkyColorHook::InstallAtOffset(0x42850);

    BiomeColorsGetAverageGrassColorHook::InstallAtOffset(0x44164);
    BiomeColorsGetAverageFoliageColorHook::InstallAtOffset(0x44174);
    BiomeColorsGetAverageSkyColorHook::InstallAtOffset(0x44194);
}