#pragma once

#include "engine/engine.h"
#include "globals.h"

class Application {
public:
    Application();

    void execute();

    Application(const Application&) = delete;
    Application(Application&&) = delete;
    Application& operator=(const Application&) = delete;
    Application& operator=(Application&&) = delete;

    ~Application();

private:
    std::unique_ptr<engine::Engine> engine_ =
        std::make_unique<engine::Engine>(globals::engineState);
};
