#pragma once

#include "engine/engine.h"
#include "engine/state.h"

struct SharedState : public engine::State {
    bool showDemoWindow = true;
};

class Application {
public:
    void run();

private:
    std::unique_ptr<engine::Engine<SharedState>> engine_;
};
