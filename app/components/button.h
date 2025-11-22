#pragma once

#include "utils/style_counter.h"

namespace components {

inline bool customButton(const char* label, ImVec2 size = ImVec2(120, 60)) {
    StyleCounter style;
    style.pushStyleColor(ImGuiCol_Button, ImVec4(0.20F, 0.30F, 0.60F, 1.0F));         // normal
    style.pushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.25F, 0.40F, 0.80F, 1.0F));  // hover
    style.pushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.15F, 0.25F, 0.55F, 1.0F));   // active

    style.pushStyleColor(ImGuiCol_Border, ImVec4(1.0F, 1.0F, 1.0F, 0.9F));  // border color
    style.pushStyleVar(ImGuiStyleVar_FrameRounding, 12.0F);                 // round corners
    style.pushStyleVar(ImGuiStyleVar_FrameBorderSize,
                       3.0F);                                        // border thickness
    style.pushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(14, 10));  // internal padding
    return ImGui::Button(label, size);
}

};
