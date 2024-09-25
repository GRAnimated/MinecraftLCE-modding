#pragma once

class Biome;

class CustomBiomes {
public:
    static Biome* BIOMES[256];
    static Biome* RAINFOREST;
    static Biome* SWAMPLAND;
    static Biome* SEASONAL_FOREST;
    static Biome* FOREST;
    static Biome* SAVANNA;
    static Biome* SHRUBLAND;
    static Biome* TAIGA;
    static Biome* DESERT;
    static Biome* PLAINS;
    static Biome* ICE_DESERT;
    static Biome* TUNDRA;

    static void createCustomBiomes();
    static void initHooks();
};
