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
    engine::Engine<SharedState> engine_;
};
