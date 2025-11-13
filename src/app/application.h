#pragma once

#include "engine/engine.h"

class Application {
public:
    void run();

private:
    engine::Engine engine_;
};
