#include "application.h"

#include <memory>

#include "engine/engine.h"
#include "globals/engine_state.h"
#include "keys/hotkeys_handler.h"
#include "layers/imgui_demo_window.h"
#include "layers/my_window.h"
#include "layers/my_window_2.h"
#include "lifetimes/globals_lifetime.h"
#include "lifetimes/surface_lifetime.h"
#include "spdlog/spdlog.h"
#include "utils/assertions.h"

Application::Application() {
    globals::engine = std::make_unique<engine::Engine>();

    auto globalsLifetime = std::make_shared<GlobalsLifetime>();
    globals::engine->pushStartupStep(globalsLifetime);
    globals::engine->pushShutdownStep(globalsLifetime);

    auto surfaceLifetime = std::make_shared<SurfaceLifetime>("Example App", 1280, 720);
    globals::engine->pushStartupStep(surfaceLifetime);
    globals::engine->pushShutdownStep(surfaceLifetime);

    globals::engine->pushRenderStep(std::make_shared<HotkeysHandler>());
    globals::engine->pushRenderStep(std::make_shared<MyWindow>());
    globals::engine->pushRenderStep(std::make_shared<ImguiDemoWindow>());
    globals::engine->pushRenderStep(std::make_shared<MyWindow2>());
}

Application::~Application() { globals::engine.reset(); }

void Application::start() {
    ASSERT(globals::engine, "only execute application with engine existing");
    spdlog::info("Running application...");
    globals::engine->runContinously();
    spdlog::info("Application stopped");
}

void Application::requestStop() {
    if (globals::engine) {
        spdlog::debug("Sending stop signal to engine...");
        globals::engine->sendStopSignal();
    }
}
