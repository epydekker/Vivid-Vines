# Code ReadMe

This code controls a set of WS2811 LED lights using an Arduino microcontroller and several input sensors, including flex sensors and touch sensors. The code allows the user to perform a variety of lighting effects and color patterns, which can be triggered by different sensor inputs.

## Libraries
The code requires the FastLED library, version 3.1 or later. Make sure to download the latest version from the [FastLED GitHub repository](https://github.com/FastLED/FastLED).

## Variables
- DATA_PIN: the pin number where the LED data is connected.
- LED_TYPE: the type of LED strip being used (in this case, WS2811).
- COLOR_ORDER: the color order of the LED strip (in this case, GRB).
- NUM_LEDS: the number of LEDs in the strip.
- BRIGHTNESS: the overall brightness of the LEDs (0-255).

## Sensor Inputs
- flexPin1, flexPin2: the analog input pins for the flex sensors.
- touchPin[]: an array of input pins for the touch sensors.
- touchVal[]: an array of variables to hold the touch sensor values.

## Functions
- setup(): initializes the code, sets up the sensor and LED pins, and configures the LED strip.
- loop(): the main loop of the code. Reads the sensor inputs and triggers different lighting effects based on the sensor values.
- idle(): a function that sets the LED strip to a low-level "idle" pattern when no other effects are being triggered.

## Lighting Effects
- fadeOn(): fades the LEDs to a specified color.
- explosion(): triggers a bright, rapid flashing pattern on the LEDs.
- microphone3(): creates a sound-reactive lighting effect based on the input from a microphone.
- strobe(): triggers a fast flashing pattern on the LEDs.
- cylon(): creates a "Knight Rider"-style back-and-forth scanning effect on the LEDs.
- pride(): creates a rainbow pattern on the LEDs.
- comet(): creates a "comet" effect, where a bright point of light travels down the strip.
- rainbowColorShow(): creates a rainbow pattern that cycles through the colors of the spectrum.
- rainbowColorShowReverse(): creates a rainbow pattern that cycles through the colors of the spectrum in reverse.
- flashEvenOdd(): flashes even and odd numbered LEDs alternately.
- rainbow(): creates a rainbow pattern that moves along the strip.
