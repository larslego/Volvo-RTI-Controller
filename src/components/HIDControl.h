#ifndef HIDCONTROL_H
#define HIDCONTROL_H

class HIDControl {
    public:
        // Attributes

        // Methods
        HIDControl();
        void update(unsigned long currentMillis);
        void pressGamePadButton(int key);
        void pressGamePadHatSwitch(int key);
        void pressConsumerButton(int key);
        void releaseAllGamePad();
        void releaseAllConsumer();

    private:
        // Attributes
        unsigned long lastPressedMillis;

        // Methods

};

#endif