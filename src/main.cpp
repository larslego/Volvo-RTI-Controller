#include <stdio.h>
#include "Arduino_FreeRTOS.h"
#include "Keyboard.h"
#include "IRremote.h"

// Keys on the IR remote that came with the car.
#define KEY_UP 3859785308
#define KEY_DOWN 3187885456
#define KEY_LEFT 1921496817
#define KEY_RIGHT 2154600643
#define KEY_ENTER 3810103859
#define KEY_BACK 3663390897

// The delay in which the screen needs a keep alive signal.
#define RTI_DELAY 100
// Pin on the ESP32.
#define IR_PIN 7

// Taskhandles
TaskHandle_t pcSerialTaskHandle; // ONLY NEEDED FOR DEBUGGING.
TaskHandle_t carSerialTaskHandle;
TaskHandle_t irTaskHandle;

// Connect:
// Screen PIN 4 to Arduino PIN 2 (TX) 
// Screen PIN 7 to Arduino ground
// Screen PIN 10 is composite video.
//https://www.swedespeed.com/threads/volvo-rti-navigation-project-with-android-odroid-platform-controlled-with-arduino.434729/
enum display_mode_name {RTI_RGB, RTI_PAL, RTI_NTSC, RTI_OFF};
const char display_modes[] = {0x40, 0x45, 0x4C, 0x46};
const char brightness_levels[] = {0x20, 0x61, 0x62, 0x23, 0x64, 0x25, 0x26, 0x67, 0x68, 0x29, 0x2A, 0x2C, 0x6B, 0x6D, 0x6E, 0x2F};

int current_display_mode = RTI_NTSC;
bool send_brightness = true;
char current_brightness_level = 13;

// Methods
void rtiWrite(char byte);
void screenNTSC();
void screenPal();
void screenRGB();
void screenOFF();
void irTask(void* parameters);
void carSerialTask(void* parameters);
void pcSerialTask(void* parameters);

void setup() {
  Serial.begin(9600); // PC
  Serial1.begin(2400); // Car

  Serial.println("Available commands: \noff: Turns off the display\npal: enables and sets display to pal mode\nrgb: enables and sets display to rgb mode\nntsc: enables and sets display to ntsc mode\n0-15: brightness");

  Keyboard.begin();

  // Create pc task
  xTaskCreate(pcSerialTask, // Task function
              "PC Serial", // Task name
              10000, // Stack size 
              NULL, // Parameters
              tskIDLE_PRIORITY, // Priority
              &pcSerialTaskHandle); // Task handler
  vTaskDelay(10);
//   Create car task
  xTaskCreate(carSerialTask, // Task function
              "Car Serial", // Task name
              10000, // Stack size 
              NULL, // Parameters
              tskIDLE_PRIORITY, // Priority
              &carSerialTaskHandle); // Task handler
  vTaskDelay(10);
//   Create infrared task
  xTaskCreate(irTask, // Task function
              "Infrared reading", // Task name
              10000, // Stack size 
              NULL, // Parameters
              tskIDLE_PRIORITY, // Priority
              &irTaskHandle); // Task handler
  vTaskDelay(10);
}

void loop() {
  // Keep watchdog from getting triggered.
  vTaskDelay(10);   
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
void pcSerialTask(void* parameters) {
  // Setup
  Serial.println("PC loop started");

  // Loop
  while (true) {
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
      vTaskDelay(RTI_DELAY / portTICK_PERIOD_MS);   
    }
  }
}

// This task writes data to the RTI display.
// The display needs a keep alive message every 100ms or so.
// In that message is the current display, brightness and an (I presume) end byte (0x83).
void carSerialTask(void* parameters) {
  // Setup
  Serial.println("Car loop started");

  // Loop
  while (true) {
    rtiWrite(display_modes[current_display_mode]);
    
    if (send_brightness) {
      rtiWrite(brightness_levels[current_brightness_level]);
    } else {
      rtiWrite(0x40);
    }
    rtiWrite(0x83); // ALWAYS NEEDS TO BE SEND LAST!
    vTaskDelay(RTI_DELAY / portTICK_PERIOD_MS);   
  }
}

// This task handles the serial data that is received from the IR remote
// on the IR sensor on the RTI display.
void irTask(void* parameters) {
  // Setup
  Serial.println("IR loop started");
  IRrecv irrecv(IR_PIN);
  // The results are received in two parts:
  // First: The button code.
  // Second: sometimes random codes. So this will not be used for now.
  decode_results results;

  irrecv.enableIRIn();

  // Loop
  while (true) {
    if (irrecv.decode(&results)) {
      if (results.value == KEY_UP) {
        //Serial.println("Up");
        //Keyboard.write(KEY_UP_ARROW);
      } else if (results.value == KEY_DOWN) {
        //Serial.println("Down");
        //Keyboard.write(KEY_DOWN_ARROW);
      } else if (results.value == KEY_LEFT) {
        //Serial.println("Left");
        //Keyboard.write(KEY_LEFT_ARROW);
      } else if (results.value == KEY_RIGHT) {
        //Serial.println("Right");
        //Keyboard.write(KEY_RIGHT_ARROW);
      } else if (results.value == KEY_BACK) {
        //Serial.println("Back");
        //Keyboard.write(KEY_ESC);
      } else if (results.value == KEY_ENTER) { // KEY_ENTER
        //Serial.println("Enter");
        //Keyboard.write(KEY_RETURN);
      }

      irrecv.resume();  // Receive the next value
      vTaskDelay(RTI_DELAY / portTICK_PERIOD_MS);
    }
  }
}
