#include <exl/nx/kernel/svc.h>
#include <mallow/config.hpp>
#include <mallow/mallow.hpp>

#include "Minecraft.Client/Minecraft.h"
#include "biome/BetaBiome.h"
#include "biome/BetaBiomeDecorator.h"
#include "biome/CustomBiomes.h"
#include "gui/DebugScreenOverlay.h"
#include "input/InputHelper.h"
#include "input/MouseAndKeyboard.h"
#include "packet/PacketDumper.h"
#include "patches/DimensionHooks.h"
#include "patches/LeafBlockLighting.h"
#include "world/BetaLevelSource.h"

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

class SelectWorldScreen {
public:
    SelectWorldScreen();

    char size[0xE0];
};

class OptionsScreen {
public:
    OptionsScreen(Screen*, Options*);

    char size[0x80];
};

struct MinecraftRunMiddleHook : public mallow::hook::Trampoline<MinecraftRunMiddleHook> {
    static void Callback(Minecraft* minecraft) {
        // if (InputHelper::isHoldR() && InputHelper::isPressA()) {
        //     minecraft->setScreen(
        //         (Screen*)new OptionsScreen((Screen*)new SelectWorldScreen(),
        //         minecraft->mOptions));
        // }
        Orig(minecraft);
    }
};

extern "C" void userMain() {
    //  mallow::config::loadConfig(true);

    // setupLogging();

    exl::hook::Initialize();

    // using Patcher = exl::patch::CodePatcher;

    // InputHelper::initKBM();
    // MouseAndKeyboard::initHooks();

    DebugScreenOverlay::initHooks();
    CustomBiomes::initHooks();
    BetaLevelSource::initHooks();
    BetaBiomeDecorator::initHooks();
    BetaBiome::initHooks();
    DimensionHooks::initHooks();
    LeafBlockLighting::initHooks();

    // PacketDumper::initHooks();

    MinecraftRunMiddleHook::InstallAtOffset(0x6DA2F0);
}
