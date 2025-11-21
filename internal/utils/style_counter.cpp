#include "style_counter.h"

#include "imgui.h"

void StyleCounter::pushStyleVar(ImGuiStyleVar styleVar, float value) {
    ImGui::PushStyleVar(styleVar, value);
    varCount_++;
}

void StyleCounter::pushStyleVar(ImGuiStyleVar styleVar, const ImVec2& values) {
    ImGui::PushStyleVar(styleVar, values);
    varCount_++;
}

void StyleCounter::pushStyleVarX(ImGuiStyleVar styleVar, float value) {
    ImGui::PushStyleVarX(styleVar, value);
    varCount_++;
}

void StyleCounter::pushStyleVarY(ImGuiStyleVar styleVar, float value) {
    ImGui::PushStyleVarY(styleVar, value);
    varCount_++;
}

void StyleCounter::pushStyleColor(ImGuiCol column, const ImVec4& color) {
    ImGui::PushStyleColor(column, color);
    colorCount_++;
}

void StyleCounter::reset() {
    clear();
    varCount_ = 0;
    colorCount_ = 0;
}

StyleCounter::~StyleCounter() { clear(); }

void StyleCounter::clear() const {
    if (varCount_ > 0) {
        ImGui::PopStyleVar(varCount_);
    }
    if (colorCount_ > 0) {
        ImGui::PopStyleColor(colorCount_);
    }
}
