#pragma once

#include <string>

namespace engine {

struct Settings {
    int height;
    int width;
    std::string title;
    bool vsync;
    float scale = 1;

    Settings(int height, int width, const std::string& title, bool vsync);
};

}
