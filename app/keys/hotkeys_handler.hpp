#pragma once

#include <spdlog/spdlog.h>

#include "globals/app_state.hpp"
#include "globals/tasks.hpp"
#include "utils/key_press_detector.hpp"

class HotkeysHandler : public engine::RenderStep {
public:
    void onRender() override {
#ifndef NDEBUG
        globals::appState->showDemoWindow = KeyPressDetector::combineKeyPressAndKeyHeld(
            f1_, f2_, globals::appState->showDemoWindow);

        if (space_.hasBeenPressed()) {
            spdlog::debug("Space has been pressed");
            if (globals::tasks->greetTask.isAvailable()) {
                globals::tasks->greetTask.execute("Alice", globals::appState->frameCount);
            } else {
                spdlog::debug("Ignored request to spawn since worker is busy");
            }
            /* if (globals::tasks->greetTask.isBusy()) {
                spdlog::debug("Canceling greet task since it's busy");
                globals::tasks->greetTask.ignore();
            }
            globals::tasks->greetTask.execute("Alice", globals::appState->frameCount); */
        }
#endif
    }

private:
    KeyPressDetector f1_{sf::Keyboard::Key::F1};
    KeyPressDetector f2_{sf::Keyboard::Key::F2};
    KeyPressDetector space_{sf::Keyboard::Key::Space};
};
