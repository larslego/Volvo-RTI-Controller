#include "vehicle.h"

Vehicle::Vehicle() {
    
}

void Vehicle::setKeyPosition(int newKeyPosition, void (*method)()) {
    if (this->cem.keyPosition != newKeyPosition) {
        if (method) (*method) ();
        this->cem.keyPosition = newKeyPosition;
    }
}

void Vehicle::setCurrentGear(int gear, void (*method)()) {
    if (this->tcm.currentGear != gear) {
        if (method) (*method) ();
        this->tcm.currentGear = gear;
    }
}

void Vehicle::setCurrentVersionByte(int byte, void (*method)()) {
    if (this->swm.currentVersionByte != byte) {
        if (method) (*method) ();
        this->swm.currentVersionByte = byte;
    }
}

void Vehicle::setSteeringWheelButton(SteeringWheelButton button, void (*method)()) {
    if (this->swm.currentButton != button) {
        if (method) (*method) ();
        this->swm.currentButton = button;
    }
}