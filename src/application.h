#pragma once

#include "engine/engine.h"

class SharedState {};

class Application {
public:
    void run();

    static void quit();

private:
    std::unique_ptr<engine::Engine<SharedState>> engine_;
};
