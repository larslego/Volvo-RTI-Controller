#include <Arduino.h>

#include "vehicle.h"
#include "components/HIDControl.h"
#include "components/display.h"
#include "Components/rtiRemote.h"
#include "components/CANbus.h"
#include "pro_micro_pins.h"

void toggleTwelveVolt();
void toggleRearViewCamera();
void checkButtonPresses();

Vehicle vehicle;
Display display;
HIDControl hidControl;
RTIRemote irRemote(&hidControl);
CANbus canBus;

int currentVersionByte = 0;

void setup() {
    pinMode(ODROID_PWR_PIN, OUTPUT);
    pinMode(RV_CAM_PIN, OUTPUT);
}

void loop() {
    canBus.update();

    if (vehicle.cem.keyPosition == 2 || vehicle.cem.keyPosition == 3) {
        display.update(millis());
        irRemote.update(millis());
        vehicle.setCurrentVersionByte(canBus.data.currentVersionByte, NULL);
        vehicle.setSteeringWheelButton(canBus.data.currentButton, checkButtonPresses);
    }

    vehicle.setKeyPosition(canBus.data.keyPosition, toggleTwelveVolt);
    vehicle.setCurrentGear(canBus.data.currentGear, toggleRearViewCamera);
}

// Enable or disable the Odroid N2+.
void toggleTwelveVolt() {
    // Using if statements so the code can correct itself if a message has been missed.
    if (vehicle.cem.keyPosition == 0 || vehicle.cem.keyPosition == 1) {
        digitalWrite(ODROID_PWR_PIN, LOW);
        display.screenOFF();
    } else if (vehicle.cem.keyPosition == 2 || vehicle.cem.keyPosition == 3) {
        digitalWrite(ODROID_PWR_PIN, HIGH);
        display.screenPAL();
    }
}

// Switch between Odroid or camera view.
void toggleRearViewCamera() {
    if (vehicle.tcm.currentGear == -1) {
        digitalWrite(RV_CAM_PIN, HIGH);
        display.screenNTSC();
    } else {
        digitalWrite(RV_CAM_PIN, LOW);
        display.screenPAL();
    }
}

void checkButtonPresses() {
    if (currentVersionByte != vehicle.swm.currentVersionByte) {
        currentVersionByte = vehicle.swm.currentVersionByte;

        switch (vehicle.swm.currentButton) {
            case DPAD_UP:
            case DPAD_RIGHT:
            case DPAD_DOWN:
            case DPAD_LEFT:
                hidControl.pressGamePadHatSwitch(vehicle.swm.currentButton);
                break;
            case DPAD_ENTER:
            case DPAD_BACK:
                hidControl.pressGamePadButton(vehicle.swm.currentButton);
                break;
            case SW_VOLUME_UP:
            case SW_VOLUME_DOWN:
            case SW_FAST_FORWARD:
            case SW_REWIND:
                hidControl.pressConsumerButton(vehicle.swm.currentButton);
                break;
            default:
                break;
        }
    }
}