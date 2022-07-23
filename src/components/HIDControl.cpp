#include "components/HIDControl.h"
#include "HID-Project.h"

HIDControl::HIDControl() {
    Gamepad.begin();
    Consumer.begin();
}

void HIDControl::update(unsigned long currentMillis) {
    if (currentMillis - lastPressedMillis > 50) {
        releaseAllConsumer();
        releaseAllGamePad();
    }
}

void HIDControl::pressGamePadButton(int key) {
    releaseAllGamePad();
    Gamepad.press(key);
    Gamepad.write();
    lastPressedMillis = millis();
}

void HIDControl::pressGamePadHatSwitch(int key) {
    releaseAllGamePad();
    if (key == 0) Gamepad.dPad1(GAMEPAD_DPAD_UP);
    else if (key == 90) Gamepad.dPad1(GAMEPAD_DPAD_RIGHT);
    else if (key == 180) Gamepad.dPad1(GAMEPAD_DPAD_DOWN);
    else if (key == 270) Gamepad.dPad1(GAMEPAD_DPAD_LEFT);
    Gamepad.write();
    lastPressedMillis = millis();
}

void HIDControl::pressConsumerButton(int key) {
    releaseAllConsumer();
    Consumer.write(static_cast<ConsumerKeycode>(key));
    lastPressedMillis = millis();
}

void HIDControl::releaseAllGamePad() {
    Gamepad.releaseAll();
}

void HIDControl::releaseAllConsumer() {
    Consumer.releaseAll();
}