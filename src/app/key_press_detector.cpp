#include "key_press_detector.h"

KeyPressDetector::KeyPressDetector(sf::Keyboard::Key key) : key_(key) {}

bool KeyPressDetector::hasBeenPressed() {
    return getStatus() == KeyPressStatus::PressStart;
}

bool KeyPressDetector::isBeingHeld() {
    KeyPressStatus status = getStatus();
    return status == KeyPressStatus::PressStart ||
           status == KeyPressStatus::PressOngoing;
}

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

bool KeyPressDetector::hasStoppedBeingPressed() {
    return getStatus() == KeyPressStatus::PressStop;
}
