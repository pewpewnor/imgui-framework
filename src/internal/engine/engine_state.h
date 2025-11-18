#pragma once

#include <atomic>

#include "SFML/Graphics/RenderWindow.hpp"

namespace engine {

struct EngineState {
    sf::RenderWindow window;

    std::atomic<bool> stopSignal;
    std::atomic<bool> refreshSignal;
};

}
