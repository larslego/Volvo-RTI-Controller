# Volvo RTI Controller

This code is mostly inspired by [this repo](https://github.com/laurynas/volvo) from [Laurynas](https://github.com/laurynas/).
His repo also has some documentation on how to hook things up. I also relied on other community efforts such as multiple forums, even more people that have projects on GitHub related to any Volvo P2 car and [a handy Google spreadsheet](https://docs.google.com/spreadsheets/d/1ZeSghcZsCCvwsY9XDJHaXvDDPIM_TntmJGmcstpcUqE).

## What the code on this repo does
- Send serial commands to the display (supports brightness, video input types and changes, turn on/off display).
- Read infrared commands from the remote (IR sensor is mounted ontop of the display).
- Convert ir remote controls to HID keyboard outputs.
- Convert button presses from the steering wheel (dpad from the RTI system, volume and fast forward/rewind). I do not want to modify the behaviour of the cruise control buttons.
- Switch a relay to swap between two composite video inputs.
- Switch a relay to toggle power to the car PC (I personally use an Odroid N2+ running android).

