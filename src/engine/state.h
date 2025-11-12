#pragma once

#include "SFML/Graphics/RenderWindow.hpp"

namespace engine {

struct State {
    std::shared_ptr<sf::RenderWindow> window;
    bool engineStopSignal = false;
};

}
