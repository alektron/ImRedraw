#include <stdint.h>
#include "sha1.h"

namespace ImRedraw
{
    const int SLEEP_IN_MS_WHEN_NO_REDRAW = 16;

    struct FrameHash
    {
        uint32_t state[5] = { 0 };
    };

    bool NeedsRedraw(FrameHash& hash)
    {
        auto drawData = ImGui::GetDrawData();

        int vertexCount = 0;
        for (int i = 0; i < drawData->CmdListsCount; i++) {
            auto cmd_list = drawData->CmdLists[i];
            vertexCount += cmd_list->VtxBuffer.size();
        }

        auto sh = sha1();
        for (int i = 0; i < drawData->CmdListsCount; i++) {
            auto cmd_list = drawData->CmdLists[i];
            sh.add(cmd_list->VtxBuffer.Data, cmd_list->VtxBuffer.size() * sizeof(ImDrawVert));
        }
        sh.finalize();
        if (memcmp(hash.state, sh.state, sizeof(sh.state)) != 0) {
            memcpy_s(hash.state, sizeof(hash.state), sh.state, sizeof(sh.state));
            return true;
        }

        return false;
    }
}

