#include "input/MouseAndKeyboard.h"

#include "mallow/hook/helpers.hpp"

#include "input/InputHelper.h"

struct CInputTickHook : public mallow::hook::Trampoline<CInputTickHook> {
    static bool Callback(void* cInput, int unk) {
        return Orig(cInput, unk);
        InputHelper::updatePadState();
    }
};

void MouseAndKeyboard::initHooks() {
    CInputTickHook::InstallAtOffset(0x78E8D0);
}
