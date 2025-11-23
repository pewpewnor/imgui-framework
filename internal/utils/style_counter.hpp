#pragma once

#include <imgui.h>

class StyleCounter {
public:
    StyleCounter() = default;
    StyleCounter(const StyleCounter&) = delete;
    StyleCounter(StyleCounter&&) = delete;
    StyleCounter& operator=(const StyleCounter&) = delete;
    StyleCounter& operator=(StyleCounter&&) = delete;

    ~StyleCounter();

    void pushStyleVar(ImGuiStyleVar styleVar, float value);

    void pushStyleVar(ImGuiStyleVar styleVar, const ImVec2& values);

    void pushStyleVarX(ImGuiStyleVar styleVar, float value);

    void pushStyleVarY(ImGuiStyleVar styleVar, float value);

    void pushStyleColor(ImGuiCol column, const ImVec4& color);

    void reset();

private:
    int varCount_ = 0;
    int colorCount_ = 0;

    void clear();
};
