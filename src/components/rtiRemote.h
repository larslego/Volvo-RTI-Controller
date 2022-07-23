#ifndef RTIREMOTE_H
#define RTIREMOTE_H

#define IR_BUTTON_COUNT 6
#define HATSWITCH_UP 0
#define HATSWITCH_DOWN 180
#define HATSWITCH_LEFT 270
#define HATSWITCH_RIGHT 90
#define BUTTON_A 0
#define BUTTON_B 1
#define IR_REMOTE_REPORT_DELAY 250

#include "components/HIDControl.h"

class RTIRemote {
    public:
        // Attributes

        // Methods
        RTIRemote(HIDControl* hid);
        void update(unsigned long currentMillis);

    private:
        // Attributes
        unsigned int irRemoteKeys[6] = { 86, 98, 90, 94, 78, 106 };
        int irKeyboardKeys[6] = { HATSWITCH_UP, HATSWITCH_DOWN, HATSWITCH_LEFT, HATSWITCH_RIGHT, BUTTON_A, BUTTON_B };
        unsigned long lastKeyReportTime = 0;
        int lastPressedKey = -1;
        HIDControl* hid;

        // Methods
};

#endif