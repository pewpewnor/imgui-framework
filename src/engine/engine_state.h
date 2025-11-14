#pragma once

#include "SFML/Graphics/RenderWindow.hpp"

namespace engine {

struct EngineState {
    sf::RenderWindow window;
    bool stopSignal;

    EngineState();
};

}

extern engine::EngineState engineState;
