# Volvo RTI Controller

This code is mostly inspired by [this repo](https://github.com/laurynas/volvo) from [Laurynas](https://github.com/laurynas/).
His repo also has some documentation on how to hook things up. ~~My code is written for the ESP32 (for now!)~~ 
*My code is now changed to run on a Arduino Pro Micro.*

## What the code on this repo does
- Send serial commands to the display (supports brightness, video input types and changes, turn on/off display).
- Read infrared commands from the remote (IR sensor is mounted ontop of the display).
- Convert ir remote controls to HID keyboard outputs.
- The Pro Micro now reads the CANbus. It can now (for example) control the power to the Odroid N2+.
- ***TODO:*** Attach the power cable from the rearview camera to an interrupt, so the controller can tell the screen to switch inputs.
- ***TODO:*** Read canbus signals from the steering wheel buttons.

