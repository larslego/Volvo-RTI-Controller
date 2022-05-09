#include <Arduino.h>
#include <Keyboard.h>
#include "IRremote.h"
#include <SPI.h>
#include <mcp2515.h>

#define IR_BUTTON_COUNT 6
unsigned int irRemoteKeys[] = { 86,     98,             90,             94,              78,         106 };
byte irKeyboardKeys[] = { KEY_UP_ARROW, KEY_DOWN_ARROW, KEY_LEFT_ARROW, KEY_RIGHT_ARROW, KEY_RETURN, KEY_ESC };

// The delay in which the screen needs a keep alive signal.
#define RTI_DELAY 100
// Delay between sending new key presses to the host.
#define HID_REPORT_DELAY 50
// Pin on the Pro Micro.
#define IR_PIN 7

// Car variables
//https://www.swedespeed.com/threads/volvo-rti-navigation-project-with-android-odroid-platform-controlled-with-arduino.434729/
enum display_mode_name {RTI_RGB, RTI_PAL, RTI_NTSC, RTI_OFF};
const char display_modes[] = {0x40, 0x45, 0x4C, 0x46};
int brightness_levels[] = {0x20, 0x61, 0x62, 0x23, 0x64, 0x25, 0x26, 0x67, 0x68, 0x29, 0x2A, 0x2C, 0x6B, 0x6D, 0x6E, 0x2F};

int current_display_mode = RTI_NTSC;
bool send_brightness = true;
int current_brightness_level = 13;
unsigned long lastRTIWrite = 0;

// IR variables
IRrecv irrecv(IR_PIN);
decode_results results;
unsigned long lastKeyReportTime = 0;
int lastPressedKey = -1;

// CANbus variables
// CANbus https://github.com/autowp/arduino-mcp2515/blob/master/examples/CAN_read/CAN_read.ino
// LOW speed
struct can_frame canMsg;
//MCP2515 mcp2515(10);

// Methods
void rtiWrite(char byte);
void screenNTSC();
void screenPal();
void screenRGB();
void screenOFF();
void irTask();
void carSerialTask();
void pcSerialTask();
void readCANbusTask();

void setup() {
  // PC setup
  Serial.begin(9600);
  while (!Serial) delay(10);
  Serial.println("Available commands: \noff: Turns off the display\npal: enables and sets display to pal mode\nrgb: enables and sets display to rgb mode\nntsc: enables and sets display to ntsc mode\n0-15: brightness");
  //Keyboard.begin();

  // Car setup
  Serial1.begin(2400);

  // IR setup
  irrecv.enableIRIn();

  // CANbus setup
  // mcp2515.reset();
  // mcp2515.setBitrate(CAN_125KBPS);
  // mcp2515.setNormalMode();
}

void loop() {
    pcSerialTask();
    carSerialTask();
    irTask();
    readCANbusTask();
    delay(15);
}

// Turns the screen off.
void screenOFF() {
  current_display_mode = RTI_OFF;
  Serial.println("Turning screen off");
}

// Sets the screen to RGB input mode.
void screenRGB() {
  current_display_mode = RTI_RGB;
  Serial.println("RGB Mode");
}

// Sets the screen to PAL input mode.
void screenPAL() {
  current_display_mode = RTI_PAL;
  Serial.println("PAL Mode");
}

// Sets the screen to NTSC input mode.
void screenNTSC() {
  current_display_mode = RTI_NTSC;
  Serial.println("NTSC Mode");
}

// Write a byte to the RTI display unit.
void rtiWrite(char byte) {
  Serial1.print(byte);
  delay(RTI_DELAY);
}

// This task handles the serial that is received from the PC.
// This task is mostly here for debug purposes.
void pcSerialTask() {
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    
    if (cmd == "off") {
      screenOFF();
    } else if (cmd == "pal") {
      screenPAL();
    } else if (cmd == "rgb") {
      screenRGB();
    } else if (cmd == "ntsc") {
      screenNTSC();
    } else if (cmd == "left") {
      Keyboard.write(KEY_LEFT_ARROW);
    } else if (cmd == "right") {
      Keyboard.write(KEY_RIGHT_ARROW);
    } else if (cmd == "enter") {
      Keyboard.write(KEY_RETURN);
    } else if (cmd == "back") {
      Keyboard.write(KEY_ESC);
    }
    
    /* else {
      send_brightness = true;
      current_brightness_level = cmd.toInt();
      send_brightness = false;
    }*/
  }
}

// This task writes data to the RTI display.
// The display needs a keep alive message every 100ms or so.
// In that message is the current display, brightness and an (I presume) end byte (0x83).
void carSerialTask() {
  unsigned long currentMillis = millis();
  if (currentMillis - lastRTIWrite > RTI_DELAY) return;

  rtiWrite(display_modes[current_display_mode]);
  
  if (send_brightness) {
    rtiWrite(brightness_levels[current_brightness_level]);
  } else {
    rtiWrite(0x40);
  }
  rtiWrite(0x83); // ALWAYS NEEDS TO BE SEND LAST!
  lastRTIWrite = currentMillis;
}

// This task handles the serial data that is received from the IR remote
// on the IR sensor on the RTI display.
void irTask() {
    if (irrecv.decode()) {
      for (int i = 0; i < IR_BUTTON_COUNT; i++) {
        if (irrecv.decodedIRData.decodedRawData == irRemoteKeys[i]) {
          unsigned long currentMillis = millis();
          // If the new key (compared to previously pressed key) is the NOT same, OR the report timeout has passed, report the new key. 
          if ((lastPressedKey == irKeyboardKeys[i] && (currentMillis - lastKeyReportTime > HID_REPORT_DELAY)) || lastPressedKey != irKeyboardKeys[i]) {
            // Report the key to the host.
            lastPressedKey = irKeyboardKeys[i];
            lastKeyReportTime = currentMillis;
            //Keyboard.write(irKeyboardKeys[i]);
            delay(1);
            Serial.print("Key: ");
            delay(1);
            Serial.println(results.value);
            break;
          }
        }
      }

      irrecv.resume();  // Receive the next value
    }
}

void readCANbusTask() {
  // 29bit
  // HS Can = 500kbps
  // LS Can = 125kbps

  // loop
    //if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK) {
      // Serial.print(canMsg.can_id, HEX); // print ID
      // Serial.print(" "); 
      // Serial.print(canMsg.can_dlc, HEX); // print DLC
      // Serial.print(" ");
      
      // for (int i = 0; i < canMsg.can_dlc; i++)  {  // print the data
      //   Serial.print(canMsg.data[i],HEX);
      //   Serial.print(" ");
      // }

      // Serial.println();      
    //}
}
