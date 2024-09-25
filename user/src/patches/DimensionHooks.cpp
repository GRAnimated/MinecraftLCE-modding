#include "patches/DimensionHooks.h"

#include "Minecraft.World/level/dimension/Dimension.h"
#include "mallow/hook/helpers.hpp"

struct DimensionUpdateLightRampHook
    : public mallow::hook::Trampoline<DimensionUpdateLightRampHook> {
    static void Callback(Dimension* dimension) {
        // return Orig(dimension);
        float f = 0.05F;
        for (int b = 0; b <= 15; b++) {
            float f1 = 1.0F - b / 15.0F;
            dimension->mBrightnessRamp[b] = (1.0F - f1) / (f1 * 3.0F + 1.0F) * (1.0F - f) + f;
        }
    }
};

void DimensionHooks::initHooks() {
    DimensionUpdateLightRampHook::InstallAtOffset(0xFAC14);
}
