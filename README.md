# Sequential-Turn-Signals
Sequential turn signals for Pontiac Fiero, and other cars with 3 rear bulbs each side. Uses an Arduino Nano, an 8 relay module, 3 12v 4 pin car relays, and some lengths of wire.

- Readme to be updated soon, this is just initial upload.

FEATURES
- Sequential signals and brake logic.
- One touch signals, will flash 3 times.
- Hazard lights flash solid, not sequentially.
- Hazard lights and brake lights only light the outer 2 bulbs, the inner most bulb is ignored so that there is a clear difference between on and off. (useful in bright daylight and with poor lenses)
- When brake is pressed with hazards on, the front turning lights and the side markers will continue to flash, while the outer two tail lights will become solid. This way it's obvious that you are both a hazard, AND slowing down.

Input Pins
- Left Signal Input - A1
- Right Signal Input - A2
- Brake Input - 3 (not A3, we want D3)

Output Pins
- Left Marker Lights - 4
- Left Outer Light - 5
- Left Middle Light - 6
- Left Inner Light - 7
- Right Inner Light - 8
- Right Middle Light - 9
- Right Outer Light - 10
- Right Marker Lights - 11

Thanks to Jason Saler for his initial project that I worked upon.
https://github.com/jasonsaler/SequentialTL/tree/master
