#pragma once

#include "engine/engine.h"

class SharedState {};

class Application {
public:
    void run();

    static void quit();

private:
    engine::Engine<SharedState> engine_;
};
