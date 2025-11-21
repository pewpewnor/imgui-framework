#pragma once

#include "SFML/Window/Keyboard.hpp"

/**
 *  @brief do not create multiple instances for the same key
 */
class KeyPressDetector {
public:
    KeyPressDetector(sf::Keyboard::Key key);

    /**
     *  @brief use this for a single state to be related to multiple keys
     *  @code showWindow = KeyPressDetector::combineKeyPresses(k1, k2,
     * showWindow);
     */
    static bool combineKeyPresses(KeyPressDetector& keyDetector1, KeyPressDetector& keyDetector2,
                                  bool state);

    /**
     *  @brief use this for a single state to be related to multiple keys
     *  @code showWindow = KeyPressDetector::combineKeyPressAndKeyHeld(k1,
     * k2, showWindow);
     */
    static bool combineKeyPressAndKeyHeld(KeyPressDetector& keyDetector1,
                                          KeyPressDetector& keyDetector2, bool state);

    /**
     *  @brief use this for a single state to be related to multiple keys
     *  @code showWindow = KeyPressDetector::combineKeyHelds(k1, k2);
     */
    static bool combineKeyHelds(KeyPressDetector& keyDetector1, KeyPressDetector& keyDetector2);

    /**
     *  @brief a single state should only be related to one key
     *  @code if (keyDetector.isPressed()) { showWindow = !showWindow; }
     *  @return true if the key has been pressed.
     */
    bool hasBeenPressed();

    /**
     *  @brief a single state should only be related to one key
     *  @code showWindow = keyDetector.isBeingHeld();
     *  @return true if the key is currently being held down.
     */
    bool isBeingHeld();

    /**
     *  @brief a single state should only be related to one key
     *  @code if (keyDetector.hasStoppedBeingHeld()) { showWindow = false; }
     * else if (keyDetector.isBeingHeld()) { showWindow = false; }
     *  @return true if the key has stopped being pressed / held
     */
    bool hasStoppedBeingPressed();

private:
    enum class KeyPressStatus { PressStart, PressOngoing, NoPress, PressStop };

    sf::Keyboard::Key key_;
    bool beingHeld_ = false;

    KeyPressStatus getStatus();
};
