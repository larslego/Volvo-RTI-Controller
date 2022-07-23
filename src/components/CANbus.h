#ifndef CANBUS_H
#define CANBUS_H

// These buttons represent that numeric value for the HID connection to other devices (like the Odroid N2+ which runs Android).
enum SteeringWheelButton {
    DPAD_UP = 0,
    DPAD_RIGHT = 90,
    DPAD_DOWN = 180,
    DPAD_LEFT = 270,
    DPAD_ENTER,
    DPAD_BACK,
    SW_VOLUME_UP,
    SW_VOLUME_DOWN,
    SW_FAST_FORWARD,
    SW_REWIND
};

struct CANData {
    int keyPosition;
    int currentGear;
    SteeringWheelButton currentButton;
    // This byte changes each time a button has been pressed or released, it does not change when a button is held pressed.
    // So it is basically an indicator to show if the data is new or old.
    int currentVersionByte;
};

class CANbus {
    public:
        // Attributes
        CANData data;

        // Methods
        CANbus();
        void update();

    private:
        // Attributes

        // Methods
};

#endif