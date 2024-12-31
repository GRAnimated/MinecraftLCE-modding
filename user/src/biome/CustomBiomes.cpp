#include "biome/CustomBiomes.h"
#include <exl/patch/code_patcher.hpp>

#include "Minecraft.World/level/material/MaterialColor.h"
#include "biome/BetaFlatBiome.h"
#include "biome/BetaRainforestBiome.h"
#include "biome/BetaSwampBiome.h"
#include "biome/BetaTaigaBiome.h"
#include "mallow/hook/helpers.hpp"

#include "Minecraft.World/level/biome/Biome.h"
#include "biome/BetaForestBiome.h"
#include "world/BetaLevelSource.h"

Biome* CustomBiomes::FOREST = nullptr;
Biome* CustomBiomes::RAINFOREST = nullptr;
Biome* CustomBiomes::SWAMPLAND = nullptr;
Biome* CustomBiomes::SEASONAL_FOREST = nullptr;
Biome* CustomBiomes::SAVANNA = nullptr;
Biome* CustomBiomes::SHRUBLAND = nullptr;
Biome* CustomBiomes::TAIGA = nullptr;
Biome* CustomBiomes::DESERT = nullptr;
Biome* CustomBiomes::PLAINS = nullptr;
Biome* CustomBiomes::ICE_DESERT = nullptr;
Biome* CustomBiomes::TUNDRA = nullptr;

Biome* CustomBiomes::BIOMES[256] = {};

void CustomBiomes::createCustomBiomes() {
    // for (int i = 0; i < 256; i++) {
    //     BIOMES[i] = Biomes::biomes[i];
    // }
    //
    // BIOMES[168] = RAINFOREST;
    // BIOMES[169] = SWAMPLAND;
    // BIOMES[170] = SEASONAL_FOREST;
    // BIOMES[171] = FOREST;
    // BIOMES[172] = SAVANNA;
    // BIOMES[173] = SHRUBLAND;
    // BIOMES[174] = TAIGA;
    // BIOMES[175] = DESERT;
    // BIOMES[176] = PLAINS;
    // BIOMES[177] = ICE_DESERT;
    // BIOMES[178] = TUNDRA;

    RAINFOREST =
        new BetaRainforestBiome((Biome::EBiomeIDs)2, new Biome::BiomeProperties(L"Rainforest"));
    RAINFOREST->setPreviewColor(Grass_Common);
    RAINFOREST->setWaterSkyColor(Water_Jungle, Water_Jungle);
    // RAINFOREST->setNameAndDescription(0x51D7F6B, 0xC285717D);

    SWAMPLAND = new BetaSwampBiome((Biome::EBiomeIDs)3, new Biome::BiomeProperties(L"Swampland"));
    SWAMPLAND->setPreviewColor(Grass_Common);
    SWAMPLAND->setWaterSkyColor(Water_Jungle, Water_Jungle);

    SEASONAL_FOREST =
        new BetaBiome((Biome::EBiomeIDs)4, new Biome::BiomeProperties(L"Seasonal Forest"));
    SEASONAL_FOREST->setPreviewColor(Grass_Common);
    SEASONAL_FOREST->setWaterSkyColor(Water_Jungle, Water_Jungle);

    FOREST = new BetaForestBiome((Biome::EBiomeIDs)5, new Biome::BiomeProperties(L"Forest"));
    FOREST->setPreviewColor(Grass_Common);
    FOREST->setWaterSkyColor(Water_Jungle, Water_Jungle);

    SAVANNA = new BetaFlatBiome((Biome::EBiomeIDs)6, new Biome::BiomeProperties(L"Savanna"));
    SAVANNA->setPreviewColor(Grass_Common);
    SAVANNA->setWaterSkyColor(Water_Jungle, Water_Jungle);

    SHRUBLAND = new BetaBiome((Biome::EBiomeIDs)7, new Biome::BiomeProperties(L"Shrubland"));
    SHRUBLAND->setPreviewColor(Grass_Common);
    SHRUBLAND->setWaterSkyColor(Water_Jungle, Water_Jungle);

    TAIGA = new BetaTaigaBiome((Biome::EBiomeIDs)8, new Biome::BiomeProperties(L"Taiga"));
    TAIGA->setPreviewColor(Grass_Common);
    TAIGA->setWaterSkyColor(Water_Jungle, Water_Jungle);

    DESERT = new BetaFlatBiome((Biome::EBiomeIDs)9, new Biome::BiomeProperties(L"Desert"));
    DESERT->setPreviewColor(Grass_Common);
    DESERT->setWaterSkyColor(Water_Jungle, Water_Jungle);

    PLAINS = new BetaFlatBiome((Biome::EBiomeIDs)10, new Biome::BiomeProperties(L"Plains"));
    PLAINS->setPreviewColor(Grass_Common);
    PLAINS->setWaterSkyColor(Water_Jungle, Water_Jungle);

    Biome::BiomeProperties* iceDesertProperties = new Biome::BiomeProperties(L"Ice Desert");
    iceDesertProperties->snow();
    ICE_DESERT = new BetaFlatBiome((Biome::EBiomeIDs)11, iceDesertProperties);
    ICE_DESERT->setPreviewColor(Grass_Common);
    ICE_DESERT->setWaterSkyColor(Water_Jungle, Water_Jungle);

    Biome::BiomeProperties* tundraProperties = new Biome::BiomeProperties(L"Tundra");
    tundraProperties->snow();
    TUNDRA = new BetaBiome((Biome::EBiomeIDs)12, tundraProperties);
    TUNDRA->setPreviewColor(Grass_Common);
    TUNDRA->setWaterSkyColor(Water_Jungle, Water_Jungle);
}

struct BiomeStaticCtorHook : mallow::hook::Trampoline<BiomeStaticCtorHook> {
    static void Callback() {
        Orig();

        CustomBiomes::createCustomBiomes();
    }
};

Biome* returnRainforest() {
    return Biome::PLAINS;
}

void CustomBiomes::initHooks() {
    BiomeStaticCtorHook::InstallAtOffset(0x3EFFC);

    exl::patch::CodePatcher patcher(0x730234);
    patcher.Write(0xEB08011F);  // CMP X8, X8
    patcher.Seek(0x730228);
    patcher.BranchLinkInst((void*)returnRainforest);
}

/*
void CustomBiomes::createCustomBiomes() {
    RAINFOREST =
        (new RainforestBiome()).setColor(588342).setName("Rainforest").setLeafColor(2094168);

    SWAMPLAND = (new SwampBiome()).setColor(522674).setName("Swampland").setLeafColor(9154376);

    SEASONAL_FOREST = (new Biome()).setColor(10215459).setName("Seasonal Forest");

    FOREST = (new ForestBiome()).setColor(353825).setName("Forest").setLeafColor(5159473);

    SAVANNA = (new FlatBiome()).setColor(14278691).setName("Savanna");

    SHRUBLAND = (new Biome()).setColor(10595616).setName("Shrubland");

    TAIGA = (new TaigaBiome())
                .setColor(3060051)
                .setName("Taiga")
                .setSnowCovered()
                .setLeafColor(8107825);

    DESERT = (new FlatBiome()).setColor(16421912).setName("Desert");

    PLAINS = (new FlatBiome()).setColor(16767248).setName("Plains");

    ICE_DESERT = (new FlatBiome())
                     .setColor(16772499)
                     .setName("Ice Desert")
                     .setSnowCovered()
                     .setLeafColor(12899129);

    TUNDRA =
        (new Biome()).setColor(5762041).setName("Tundra").setSnowCovered().setLeafColor(12899129);
}
*/
