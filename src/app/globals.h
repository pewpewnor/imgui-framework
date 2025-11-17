#pragma once

#include <memory>

#include "async_worker.h"
#include "engine/engine_state.h"

struct AppState {
    bool showDemoWindow = false;
    AsyncWorker<std::string> sleepWorker;
    std::string greetings;
};

namespace globals {

inline std::shared_ptr<AppState> appState;

inline std::shared_ptr<engine::EngineState> engineState;

};
