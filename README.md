# Arduino-Sleep-Timer


A small Arduino project that puts the board into low-power sleep mode after a configurable timeout and wakes on button press (or other interrupt). Useful for battery-powered projects where you want to conserve power when the device is idle.

## Features
- Configurable sleep timeout (seconds/minutes)
- Uses Arduino low-power sleep modes to reduce current draw
- Wake from external interrupt (button) or RTC / timer (depending on hardware)
- Simple wiring and easy-to-adapt sketch

## Hardware
- Arduino Uno, Nano, or any AVR/ARM board that supports sleep modes
- Pushbutton (for wake-up) or compatible RTC/module if needed
- Optional: pull-down/up resistor for button (10k recommended)

## Wiring
- Button between a digital interrupt pin (e.g., D2 on Uno) and GND (or VCC if using INPUT_PULLUP)
- If using INPUT_PULLUP, connect button between pin and GND
- If using external pull-down, connect resistor from pin to GND and button to VCC

Example for Uno using D2 (INT0) and INPUT_PULLUP:
- D2 -> Button -> GND

## Usage
1. Open the provided sketch in Arduino IDE.
2. Adjust the sleep timeout or pin definitions at the top of the sketch.
3. Upload to your board.
4. Press the button to wake the board from sleep.



## Notes and tips
- For lowest power, disable unused peripherals before sleeping (ADC, TWI, timers) and re-enable after wake.
- If using an RTC (e.g., DS3231) or low-power timer, you can wake the MCU without user input.
- Test wake behavior with Serial messages removed or slowed — Serial can keep peripherals active.

## License
This project is MIT-licensed. See LICENSE for details.
