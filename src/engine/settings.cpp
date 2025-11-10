#include "settings.h"

engine::Settings::Settings(int32_t height, int32_t width,
                           const std::string& title, bool vsync)
    : height(height), width(width), title(title), vsync(vsync) {}
