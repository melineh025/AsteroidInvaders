# AsteroidInvaders
Play against three enemy spaceships and avoid getting hit!

Demo:

<img src="https://media.giphy.com/media/imXZNAOa5qZz2JGhiW/giphy-downsized-large.gif"  />

Hardware required:
- Arduino Uno
- LED Matrix ( 16 x 16)
- Breadboard / wiring / wires
- Buttons and joystick, or combined module


Connections for Arduino and Combined Module:
- Key A- D2
- Key B- D3
- Key C-D4
- Key D-D5
- Key E- D6
- Key F- D7
- Analog Pins A0 to JoyX
- Analog Pins A1 to JoyY
- GND to Arduino GND

Connections for Arduino and LED Matrix:
- 5V to 5V
- Digital Pin D8
- GND to GND

Setup:
- Copy/Paste and compile the C++ code inside the Arudino IDE
- Import LED Matrix library and other requirements
- Check if it compiles successfully, warnings are okay
- Read your Microcontroller's interrupt system and calibrate (Uno is calibrated above)
