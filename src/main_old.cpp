#include <Arduino.h>
#include <mcp2515.h>

// #include "pro_micro_pins.h"
// #include "canbus_ids.h"
// #include "canbus_msgs.h"

// #define IR_BUTTON_COUNT 6
// unsigned int irRemoteKeys[] = { 86, 98, 90, 94, 78, 106 };
// #define HATSWITCH_UP 0
// #define HATSWITCH_DOWN 180
// #define HATSWITCH_LEFT 270
// #define HATSWITCH_RIGHT 90
// #define BUTTON_A 0
// #define BUTTON_B 1
// #define BUTTON_X 2
// #define BUTTON_Y 3
// int irKeyboardKeys[] = { HATSWITCH_UP, HATSWITCH_DOWN, HATSWITCH_LEFT, HATSWITCH_RIGHT, BUTTON_A, BUTTON_B };

// Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_GAMEPAD, 2, 1,
// false, false, false, false, false, false, false, false, false, false, false);

// // The delay in which the screen needs a keep alive signal.
// #define RTI_DELAY 50
// // Delay between sending new key presses to the host.
// #define HID_REPORT_DELAY 250

// // Car variables
// //https://www.swedespeed.com/threads/volvo-rti-navigation-project-with-android-odroid-platform-controlled-with-arduino.434729/
// enum display_mode_name {RTI_RGB, RTI_PAL, RTI_NTSC, RTI_OFF};
// const char display_modes[] = {0x40, 0x45, 0x4C, 0x46};
// int brightness_levels[] = {0x20, 0x61, 0x62, 0x23, 0x64, 0x25, 0x26, 0x67, 0x68, 0x29, 0x2A, 0x2C, 0x6B, 0x6D, 0x6E, 0x2F};

// int current_display_mode = RTI_NTSC;
// bool send_brightness = true;
// int current_brightness_level = 13;
// unsigned long lastRTIWrite = 0;

// bool isCarTurnedOff = true;

// // IR variables
// IRrecv irrecv(-1);
// decode_results results;
// unsigned long lastKeyReportTime = 0;
// int lastPressedKey = -1;

// // CANbus variables
// // CANbus https://github.com/autowp/arduino-mcp2515/blob/master/examples/CAN_read/CAN_read.ino
// // LOW speed
// struct can_frame canMsg;
// MCP2515 mcp2515(CANBUS_CS_PIN);

// // Methods
// void rtiWrite(char byte);
// void screenNTSC();
// void screenPAL();
// void screenRGB();
// void screenOFF();
// void irTask();
// void carSerialTask();
// void pcSerialTask();
// void readCANbusTask();
// void onKeyStateRead(int newKeyState);
// void pressButton(int key);
// void pressHatswitch(int key);

// void setup() {
//   // Joystick setup
//   // Serial.begin(9600);
//   // delay(2);
//   // while (!Serial) delay(20);

//   Joystick.begin();
//   delay(15);

//   // Car setup
//   Serial1.begin(2400);
//   pinMode(RV_CAM_PIN, OUTPUT);
//   pinMode(ODROID_PWR_PIN, OUTPUT); // Enable relay control.
//   onKeyStateRead(-1);
//   delay(15);

//   // IR setup
//   irrecv.enableIRIn();

//   // CANbus setup
//   mcp2515.reset();
//   mcp2515.setBitrate(CAN_125KBPS, MCP_8MHZ);
//   mcp2515.setConfigMode();
//   mcp2515.setFilterMask(MCP2515::MASK0, true, 0x00000000);
//   mcp2515.setFilter(MCP2515::RXF0, true, 0x00000000);
//   mcp2515.setFilter(MCP2515::RXF1, true, 0x00000000);
//   mcp2515.setFilterMask(MCP2515::MASK1, true, 0x00000000);
//   mcp2515.setFilter(MCP2515::RXF2, true, 0x00000000);
//   mcp2515.setFilter(MCP2515::RXF3, true, 0x00000000);
//   mcp2515.setFilter(MCP2515::RXF4, true, 0x00000000);
//   mcp2515.setFilter(MCP2515::RXF5, true, 0x00000000);
//   mcp2515.setListenOnlyMode();
// }

// void loop() {
//   if (!isCarTurnedOff) {
//     delay(15);
//     carSerialTask();
//     irTask();
//   }

//   readCANbusTask();
// }

// // Turns the screen off.
// void screenOFF() {
//   current_display_mode = RTI_OFF;
// }

// // Sets the screen to RGB input mode.
// void screenRGB() {
//   current_display_mode = RTI_RGB;
// }

// // Sets the screen to PAL input mode.
// void screenPAL() {
//   current_display_mode = RTI_PAL;
// }

// // Sets the screen to NTSC input mode.
// void screenNTSC() {
//   current_display_mode = RTI_NTSC;
// }

// // Write a byte to the RTI display unit.
// void rtiWrite(char byte) {
//   Serial1.print(byte);
//   delay(RTI_DELAY);
// }

// // This task handles the serial that is received from the PC.
// // This task is mostly here for debug purposes.
// void pcSerialTask() {}

// // This task writes data to the RTI display.
// // The display needs a keep alive message every 100ms or so.
// // In that message is the current display, brightness and an (I presume) end byte (0x83).
// void carSerialTask() {
//   unsigned long currentMillis = millis();
//   if (currentMillis - lastRTIWrite < RTI_DELAY) return;

//   rtiWrite(display_modes[current_display_mode]);
  
//   if (send_brightness) rtiWrite(brightness_levels[current_brightness_level]);
//   else rtiWrite(0x40);
  
//   rtiWrite(0x83); // ALWAYS NEEDS TO BE SEND LAST!
//   lastRTIWrite = currentMillis;
// }

// // This task handles the serial data that is received from the IR remote
// // on the IR sensor on the RTI display.
// void irTask() {
//   if (irrecv.decode()) {
//     for (int i = 0; i < IR_BUTTON_COUNT; i++) {
//       if (irrecv.decodedIRData.decodedRawData == irRemoteKeys[i]) {
//         unsigned long currentMillis = millis();
//         // If the new key (compared to previously pressed key) is the NOT same, OR the report timeout has passed, report the new key. 
//         if ((lastPressedKey == irKeyboardKeys[i] && (currentMillis - lastKeyReportTime > HID_REPORT_DELAY)) || lastPressedKey != irKeyboardKeys[i]) {
//           // Report the key to the host.
//           lastPressedKey = irKeyboardKeys[i];
//           lastKeyReportTime = currentMillis;

//           // Determine if the key is a button or dpad button.
//           if (i >= 4) pressButton(irKeyboardKeys[i]);
//           else pressHatswitch(irKeyboardKeys[i]);
//           break;
//         }
//       }
//     }

//     irrecv.resume();  // Receive the next value
//   }
// }

// void readCANbusTask() {
//   // 29bit
//   // HS Can = 500kbps
//   // LS Can = 125kbps
//   if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {
//     if ((canMsg.can_id & 0x1FFFFFFF) == CEM) {
//       if (canMsg.data[4] == KEY_0) onKeyStateRead(0);
//       else if (canMsg.data[4] == KEY_1) onKeyStateRead(1);
//       else if (canMsg.data[4] == KEY_2) onKeyStateRead(2);
//       else if (canMsg.data[4] == NO_KEY) onKeyStateRead(-1);  
//     }

//     if ((canMsg.can_id & 0x1FFFFFFF) == STEERING_WHEEL_MODULE) {
//       if (canMsg.data[7] == VOLUME_UP);
//       else if (canMsg.data[7] == VOLUME_DOWN);
//       else if (canMsg.data[7] == PREVIOUS_TRACK) pressButton(BUTTON_Y);
//       else if (canMsg.data[7] == NEXT_TRACK) pressButton(BUTTON_X);
//     }

//     if ((canMsg.can_id & 0x1FFFFFFF) == STEERING_WHEEL_MODULE) {
//       if (canMsg.data[6] == RTI_BUTTON_LEFT) pressHatswitch(HATSWITCH_LEFT);
//       else if (canMsg.data[6] == RTI_BUTTON_RIGHT) pressHatswitch(HATSWITCH_RIGHT);
//       else if (canMsg.data[6] == RTI_BUTTON_UP) pressHatswitch(HATSWITCH_UP);
//       else if (canMsg.data[6] == RTI_BUTTON_DOWN) pressHatswitch(HATSWITCH_DOWN);
//       else if (canMsg.data[6] == RTI_BUTTON_ENTER) pressButton(BUTTON_A);
//       else if (canMsg.data[6] == RTI_BUTTON_BACK) pressButton(BUTTON_B);
//     }

//     if ((canMsg.can_id & 0x1FFFFFFF) == GEARBOX) {
//       if (canMsg.data[2] == 1) digitalWrite(RV_CAM_PIN, 1);
//       else digitalWrite(RV_CAM_PIN, 0);
//     }

//     // if ((canMsg.can_id & 0x1FFFFFFF) == STEERING_WHEEL_MODULE) {
//     //   if (canMsg.data[6] == 0) return;
//     //   //if (true) {
//     //   delay(2);
//     //   Serial.print((canMsg.can_id & 0x1FFFFFFF), HEX);
//     //   delay(2);
    
//     //   Serial.print(" "); 
//     //   delay(2);
//     //   Serial.print(canMsg.can_dlc, HEX); // print DLC
//     //   delay(2);
//     //   Serial.print(" ");
//     //   delay(2);
      
//     //   //Serial.println(canMsg.data[6, HEX]);

//     //   for (int i = 0; i < canMsg.can_dlc; i++)  {  // print the data
//     //   delay(2);
//     //     Serial.print(canMsg.data[i],HEX);
//     //     delay(2);
//     //     Serial.print(" ");
//     //   }
//     //   Serial.println();
//     // }
    
//     // }
//   }
// }

// void onKeyStateRead(int newKeystate) {
//   if (/*newKeystate == 1 || */newKeystate == 0 || newKeystate == -1) {
//      //Turn relay off.
//     digitalWrite(ODROID_PWR_PIN, LOW);
//     screenOFF();
//     isCarTurnedOff = true;
//   } else if (newKeystate == 2 || newKeystate == 1) {
//     // Turn relay on.
//     digitalWrite(ODROID_PWR_PIN, HIGH);
//     isCarTurnedOff = false;
//     //screenPAL();
//   }
// }

// void pressButton(int key) {
//   Joystick.pressButton(key);
//   delay(10);
//   Joystick.releaseButton(key);    
// }

// void pressHatswitch(int key) {
//   Joystick.setHatSwitch(0, key);
//   delay(10);
//   Joystick.setHatSwitch(0, -1);
// }