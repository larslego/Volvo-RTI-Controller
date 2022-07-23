#ifndef display_h
#define display_h

#define RTI_DELAY 50
class Display {
    public:
        // Attributes

        // Methods
        Display();
        void update(unsigned long currentMillis);
        void screenNTSC();
        void screenPAL();
        void screenRGB();
        void screenOFF();

    private:
        // Attributes
        enum display_mode_name {RTI_RGB, RTI_PAL, RTI_NTSC, RTI_OFF};
        char display_modes[4] = {0x40, 0x45, 0x4C, 0x46};
        int brightness_levels[16] = {0x20, 0x61, 0x62, 0x23, 0x64, 0x25, 0x26, 0x67, 0x68, 0x29, 0x2A, 0x2C, 0x6B, 0x6D, 0x6E, 0x2F};
        int current_display_mode = RTI_NTSC;
        bool send_brightness = true;
        int current_brightness_level = 13;
        unsigned long lastRTIWrite = 0;

        // Methods
        void rtiWrite(char byte);
};

#endif