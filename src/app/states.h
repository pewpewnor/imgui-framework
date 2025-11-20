#pragma once

#include <memory>

#include "app_state.h"
#include "engine/engine.h"

namespace globals {

inline std::unique_ptr<engine::Engine> engine;

inline std::unique_ptr<AppState> appState;

};
