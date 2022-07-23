#ifndef CANBUS_MSGS_H
#define CANBUS_MSGS_H
#endif

// Byte indexes are counted starting from 1 (not zero).
#define LEFT_STALK_READ_BUTTON 0xA0

// CEM
#define KEY_0 0xE0
#define KEY_1 0xE8
#define KEY_2 0xF0
#define NO_KEY 0xC0

//AEM
// Media keys are differentiated on byte 7 from the data.
#define PREVIOUS_TRACK 0x3E
#define NEXT_TRACK 0x3D
#define VOLUME_UP 0x37
#define VOLUME_DOWN 0x3B

//SWM
// RTI (byte 7). Byte 2 changes when a key has been released or when a different key has been pressed.
//  |-This means you probably want to release the software keys when byte 2 changes from its previous value.
#define RTI_BUTTON_ENTER 20
#define RTI_BUTTON_BACK 10
#define RTI_BUTTON_UP 8
#define RTI_BUTTON_DOWN 4
#define RTI_BUTTON_LEFT 1
#define RTI_BUTTON_RIGHT 2