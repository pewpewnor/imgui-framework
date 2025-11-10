#pragma once

#include <string>

namespace engine {

struct Settings {
    int height = 1280;
    int width = 720;
    std::string title;
    bool vsync = true;

    Settings() = default;

    Settings(int32_t height, int32_t width, const std::string& title,
             bool vsync);
};

}
