#pragma once

#include "imgui.h"

class StyleStack {
public:
    StyleStack() = default;

    void pushStyleVar(ImGuiStyleVar styleVar, float value);

    void pushStyleVar(ImGuiStyleVar styleVar, const ImVec2& values);

    void pushStyleVarX(ImGuiStyleVar styleVar, float value);

    void pushStyleVarY(ImGuiStyleVar styleVar, float value);

    void pushStyleColor(ImGuiCol column, const ImVec4& color);

    void reset();

    StyleStack(const StyleStack&) = delete;
    StyleStack(StyleStack&&) = delete;
    StyleStack& operator=(const StyleStack&) = delete;
    StyleStack& operator=(StyleStack&&) = delete;

    ~StyleStack();

private:
    int varCount_ = 0;
    int colorCount_ = 0;

    void clear() const;
};
