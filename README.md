# MacroPadRP2040Driver
A trivial python based Adafruit MacroPadRP2040 Driver that registers itself as a HID device and sends reports depending on the pressed buttons.

# Installation
1. Download [CircuitPython](https://circuitpython.org/board/adafruit_macropad_rp2040/). I used 9.1.4, but I guess any should do.
2. Plug the MacroPadRP2040 into your PC.
3. Hold down the audio knob (rotation button thingy on the top right, technical term: "encoder_switch_debounced") and the reset button (left side just under the screen) at the same time. Release reset button, then audio knob.
4. On the PC, the MacroPadRP2040 should have been recognized as a drive. Copy the downloaded .UF2 file from step 1 onto the board. This will automatically restart the board. The board is now recognized as "CIRCUITPY".
5. Download [libraries](https://circuitpython.org/libraries). Make sure to use the correct bundle, depending on what you downloaded on step 1.
6. Within the bundle, navigate to the lib directory.
7. Within CIRCUITPY, navigate to the lib directory as well.
8. Copy over:
  * adafruit_display_text (directory)
  * adafruit_hid (directory)
  * adafruit_midi (directory)
  * adafruit_debouncer.mpy
  * adafruit_macropad.mpy
  * adafruit_simple_text_display.mpy
  * adafruit_ticks.mpy
  * neopixel.mpy
  * neopixel_spi.mpy
9. Within CIRCUITPY, navigate to the main directory.
10. Copy over the boot.py and code.py from this repository into the main directory of CIRCUITPY
11. Press the reset Button on the MacroPadRP2040 (might not be necessary, as the MacroPad restarts automatically when boot/code.py are changed, but just to be sure)

All done! Your MacroPadRP2040 is now using the driver from this repo. Within the Example Directory, there is a Windows C++ Program that is reading the button presses and prints the raw date to the console. Not really "production ready", but it should explain how to use the MacroPadRP2040 quite well.
