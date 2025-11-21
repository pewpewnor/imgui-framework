#include "key_press_detector.h"

KeyPressDetector::KeyPressDetector(sf::Keyboard::Key key) : key_(key) {}

bool KeyPressDetector::combineKeyPresses(KeyPressDetector& keyDetector1,
                                         KeyPressDetector& keyDetector2, bool state) {
    if (keyDetector1.hasBeenPressed() || keyDetector2.hasBeenPressed()) {
        return !state;
    }
    return state;
}

bool KeyPressDetector::combineKeyPressAndKeyHeld(KeyPressDetector& keyDetector1,
                                                 KeyPressDetector& keyDetector2, bool state) {
    if (keyDetector2.hasStoppedBeingPressed()) {
        return false;
    }
    if (keyDetector2.isBeingHeld()) {
        return true;
    }
    if (keyDetector1.hasBeenPressed()) {
        return !state;
    }
    return state;
}

bool combineKeyHelds(KeyPressDetector& keyDetector1, KeyPressDetector& keyDetector2) {
    return keyDetector1.isBeingHeld() || keyDetector2.isBeingHeld();
}

bool KeyPressDetector::hasBeenPressed() { return getStatus() == KeyPressStatus::PressStart; }

bool KeyPressDetector::isBeingHeld() {
    KeyPressStatus status = getStatus();
    return status == KeyPressStatus::PressStart || status == KeyPressStatus::PressOngoing;
}

bool KeyPressDetector::hasStoppedBeingPressed() { return getStatus() == KeyPressStatus::PressStop; }

KeyPressDetector::KeyPressStatus KeyPressDetector::getStatus() {
    if (sf::Keyboard::isKeyPressed(key_)) {
        if (!beingHeld_) {
            beingHeld_ = true;
            return KeyPressStatus::PressStart;
        }
        return KeyPressStatus::PressOngoing;
    }
    bool prevBeingHeld = beingHeld_;
    beingHeld_ = false;
    return prevBeingHeld ? KeyPressStatus::PressStop : KeyPressStatus::NoPress;
}
