#pragma once

#include <memory>

#include "engine/engine_state.h"

struct AppState {
    bool showDemoWindow = false;
};

namespace globals {

inline std::shared_ptr<AppState> appState;

inline std::shared_ptr<engine::EngineState> engineState;

};
