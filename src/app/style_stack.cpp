#include "style_stack.h"

#include "imgui.h"

void StyleStack::pushStyleVar(ImGuiStyleVar styleVar, float value) {
    ImGui::PushStyleVar(styleVar, value);
    varCount_++;
}

void StyleStack::pushStyleVar(ImGuiStyleVar styleVar, const ImVec2& values) {
    ImGui::PushStyleVar(styleVar, values);
    varCount_++;
}

void StyleStack::pushStyleVarX(ImGuiStyleVar styleVar, float value) {
    ImGui::PushStyleVarX(styleVar, value);
    varCount_++;
}

void StyleStack::pushStyleVarY(ImGuiStyleVar styleVar, float value) {
    ImGui::PushStyleVarY(styleVar, value);
    varCount_++;
}

void StyleStack::pushStyleColor(ImGuiCol column, const ImVec4& color) {
    ImGui::PushStyleColor(column, color);
    colorCount_++;
}

void StyleStack::reset() {
    clear();
    varCount_ = 0;
    colorCount_ = 0;
}

StyleStack::~StyleStack() { clear(); }

void StyleStack::clear() const {
    if (varCount_ > 0) {
        ImGui::PopStyleVar(varCount_);
    }
    if (colorCount_ > 0) {
        ImGui::PopStyleColor(colorCount_);
    }
}
