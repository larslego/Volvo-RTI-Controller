# Volvo-RTI-Controller

This code is mostly inspired by [this repo](https://github.com/laurynas/volvo) from [Laurynas](https://github.com/laurynas/).
His repo also has some documentation on how to hook things up. My code is written for the ESP32 (for now!).

My version of his code added support for the infrared remote. These IR signal are then converted to a bluetooth signal.
The ESP32 is seen as a ble keyboard for other devices. 

***I KNOW THAT THIS IS NOT A GOOD SOLUTION BUT I DO NOT HAVE A REPLACEMENT BOARD YET***
