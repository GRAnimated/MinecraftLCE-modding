#include "patches/LeafBlockLighting.h"

#include <exl/patch/code_patcher.hpp>
#include "mallow/hook/helpers.hpp"

#include "Minecraft.World/level/block/Blocks.h"
#include "Minecraft.World/level/storage/LevelSource.h"

struct BlockRendererGetShadeBrightnessHook
    : mallow::hook::Trampoline<BlockRendererGetShadeBrightnessHook> {
    static float Callback(void* blockRenderer, Block* block, LevelSource* source,
                          const BlockPos& pos) {
        const BlockState* state = source->getBlock(pos)->defaultBlockState();
        if (state == Blocks::LEAVES->defaultBlockState() ||
            state == Blocks::LEAVES2->defaultBlockState()) {
            return 1.0f;
        }
        return Orig(blockRenderer, block, source, pos);
    }
};

struct LeafBlockIsSolidRenderer : mallow::hook::Trampoline<LeafBlockIsSolidRenderer> {
    static bool Callback(Block* leafBlock, const BlockState* state) { return false; }
};

struct LeafBlockSetFancyRenderer : mallow::hook::Trampoline<LeafBlockSetFancyRenderer> {
    static void Callback(Block* block, bool isFancy) { Orig(block, true); }
};

void LeafBlockLighting::initHooks() {
    BlockRendererGetShadeBrightnessHook::InstallAtOffset(0x618878);

    LeafBlockIsSolidRenderer::InstallAtOffset(0x1F77E4);
    LeafBlockSetFancyRenderer::InstallAtOffset(0x1F77F4);

    exl::patch::CodePatcher Patcher(0x1F78C8);  // LeafBlock::shouldRenderFace
    Patcher.WriteInst(0x52800020);              // MOV W0, #1
}
