#pragma once

#include "globals/app_state.h"
#include "globals/tasks.h"
#include "spdlog/spdlog.h"
#include "utils/key_press_detector.h"

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
