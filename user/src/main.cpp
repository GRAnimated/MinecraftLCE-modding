#include <mallow/config.hpp>
#include <mallow/mallow.hpp>

#include "biome/BetaBiome.h"
#include "biome/BetaBiomeDecorator.h"
#include "gui/DebugScreenOverlay.h"
#include "patches/DimensionHooks.h"

#include "Minecraft.Client/Minecraft.h"
#include "biome/CustomBiomes.h"
#include "patches/LeafBlockLighting.h"
#include "patches/MouseAndKeyboard.h"
#include "world/CustomLevelSource.h"

static void setupLogging() {
    using namespace mallow::log::sink;
    // This sink writes to a file on the SD card.
    static FileSink fileSink = FileSink("sd:/mallow.log");
    addLogSink(&fileSink);

    // This sink writes to a network socket on a host computer. Raw logs are sent with no
    auto config = mallow::config::getConfig();
    if (config["logger"]["ip"].is<const char*>()) {
        static NetworkSink networkSink =
            NetworkSink(config["logger"]["ip"], config["logger"]["port"] | 3080);
        if (networkSink.isSuccessfullyConnected())
            addLogSink(&networkSink);
        else
            mallow::log::logLine("Failed to connect to the network sink");
    } else {
        mallow::log::logLine("The network logger is unconfigured.");
        if (config["logger"].isNull()) {
            mallow::log::logLine("Please configure the logger in config.json");
        } else if (!config["logger"]["ip"].is<const char*>()) {
            mallow::log::logLine("The IP address is missing or invalid.");
        }
    }
}

extern "C" void userMain() {
    // nn::fs::MountSdCardForDebug("sd");
    // mallow::config::loadConfig(true);

    // setupLogging();

    exl::hook::Initialize();

    // using Patcher = exl::patch::CodePatcher;

    // DebugScreenOverlay::initHooks();
    CustomBiomes::initHooks();
    CustomLevelSource::initHooks();
    BetaBiomeDecorator::initHooks();
    BetaBiome::initHooks();
    DimensionHooks::initHooks();
    LeafBlockLighting::initHooks();
    MouseAndKeyboard::initHooks();
}
