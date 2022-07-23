#ifndef vehicle_h
#define vehicle_h

#include "components/CANbus.h"

struct CEM {
    int keyPosition;
};

struct SWM {
    SteeringWheelButton currentButton;
    // This byte changes each time a button has been pressed or released, it does not change when a button is held pressed.
    // So it is basically an indicator to show if the data is new or old.
    int currentVersionByte;
};

struct TCM {
    int currentGear;
};

class Vehicle {
    public:
        // Attributes
        struct CEM cem;
        struct SWM swm;
        struct TCM tcm;

        // Methods
        Vehicle();
        void setKeyPosition(int newKeyPosition, void (*method)());
        void setCurrentGear(int gear, void (*method)());
        void setSteeringWheelButton(SteeringWheelButton button, void (*method)());
        void setCurrentVersionByte(int byte, void (*method)());
    
    private:
        // Attributes

        // Methods

};

#endif