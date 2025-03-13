 LILYGO T-Display-S3 Automatic Flashing LED Project

 **!! IMPORTANT !!**
  - This module uses 2 pins for operation >> (S) pin & (-) pin <<
  - Only connect GND (-) and VCC (S) to your dev board -- **DO NOT connect the middle pin to anything!**
  - The module's (-) and (middle) pins are SHORTED with solder by design and can DAMAGE your board if connected to 3V or (especially) 5V power.

 Description:

   This code controls a KY-034 Automatic Flashing Colour LED connected to a LilyGO T-Display-S3.
   The LED is powered through the "S" pin connected to GPIO01 and flashes automatically when powered.
   - The KEY button (GPIO14) turns on the LED only while it’s held down, but is ignored if the BOOT button has toggled the LED state.
   - The BOOT button (GPIO0) toggles the LED state, keeping it on or off until toggled again.
   
   The LED state (ON/OFF) is displayed on the built-in screen using the TFT_eSPI library.

 How It Works:
   1. LED Control: The KY-034 LED is powered through GPIO01 as an output.
   2. Button Interaction:
      - KEY Button (GPIO14): While the button is pressed, GPIO01 is set HIGH, powering the LED, but only if the BOOT button has not toggled the LED state.
      - BOOT Button (GPIO0): Toggles the LED state, keeping it on or off until toggled again.
   3. TFT_eSPI Display: The screen displays the current LED state (ON/OFF) and updates dynamically without redrawing the entire screen.

 Pin Connections:
  - KY-034 S      -> GPIO01 (or any available GPIO pin)
  - KY-034 -      -> GND
  - KEY Button    -> GPIO14 (built-in KEY button)
  - BOOT Button   -> GPIO0 (built-in BOOT button)
  - LCD Backlight -> GPIO15

 Notes:
  - The KY-034 LED module flashes automatically when powered, so we control its power using GPIO01.
  - The TFT_eSPI library is configured to work with the LilyGO T-Display-S3.
  - The screen updates only the LED state text to improve performance.
