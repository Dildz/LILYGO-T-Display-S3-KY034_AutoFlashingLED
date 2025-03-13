/*********************************************************************************************************
 * LILYGO T-Display-S3 Automatic Flashing LED Project
 *
 * Description:
 *   This code controls a KY-034 Automatic Flashing Colour LED connected to a LilyGO T-Display-S3.
 *   The LED is powered through the `S` pin connected to GPIO01 and flashes automatically when powered.
 *   - The KEY button (GPIO14) turns on the LED only while it’s held down, but is ignored if the BOOT
 *     button has toggled the LED state.
 *   - The BOOT button (GPIO0) toggles the LED state, keeping it on or off until toggled again.
 *   The LED state (ON/OFF) is displayed on the built-in screen using the TFT_eSPI library.
 *
 * How It Works:
 *   1. LED Control: The KY-034 LED is powered through GPIO01 as an output.
 *   2. Button Interaction:
 *      - KEY Button (GPIO14): While the button is pressed, GPIO01 is set HIGH, powering the LED, but only
 *        if the BOOT button has not toggled the LED state.
 *      - BOOT Button (GPIO0): Toggles the LED state, keeping it on or off until toggled again.
 *   3. TFT_eSPI Display: The screen displays the current LED state (ON/OFF) and updates dynamically
 *      without redrawing the entire screen.
 *
 * Core Concepts:
 *  - Direct Button Handling: Read the button states directly without debouncing.
 *  - TFT_eSPI Library: Used to display the LED state on the screen.
 *  - Efficient Screen Updates: Only the LED state text is updated to avoid unnecessary redraws.
 *
 * Pin Connections:
 *  - KY-034 S      -> GPIO01 (or any available GPIO pin)
 *  - KY-034 -      -> GND
 *  - KEY Button    -> GPIO14 (built-in KEY button)
 *  - BOOT Button   -> GPIO0 (built-in BOOT button)
 *  - LCD Backlight -> GPIO15
 *
 * Notes:
 *  - The KY-034 LED module flashes automatically when powered, so we control its power using GPIO01.
 *  - The TFT_eSPI library is configured to work with the LilyGO T-Display-S3.
 *  - The screen updates only the LED state text to improve performance.
 * 
 * !! IMPORTANT !!
 * - This module uses 2 pins for operation.
 * - Only connect GND (-) and VCC (S) to your dev board -- DO NOT connect the middle pin!
 * - The module's (-) and (middle) pins are SHORTED with solder and can DAMAGE your board if connected to
 *   3V or (especially) 5V power.
 *********************************************************************************************************/

/*************************************************************
******************* INCLUDES & DEFINITIONS *******************
**************************************************************/

#include <Arduino.h>
#include <TFT_eSPI.h>

// Pin Definitions
#define LED_PIN 1     // GPIO01 (connected to KY-034 S pin)
#define KEY_BUTTON 14 // KEY button (GPIO14)
#define BOOT_BUTTON 0 // BOOT button (GPIO0)
#define PIN_LCD_BL 15 // LCD backlight (GPIO15)

// TFT_eSPI object
TFT_eSPI tft = TFT_eSPI();

// Global Variables
bool ledState = false;           // current LED state (ON/OFF)
bool bootToggled = false;        // flag to indicate if BOOT button has toggled the LED state
bool lastKeyButtonState = HIGH;  // last state of the KEY button (active LOW)
bool lastBootButtonState = HIGH; // last state of the BOOT button (active LOW)
bool redrawRequired = true;      // flag to indicate if the screen needs updating


/*************************************************************
********************** HELPER FUNCTIONS **********************
**************************************************************/

// Function to draw static elements on the TFT screen
void drawStaticElements() {
  // Clear the screen and set text properties
  tft.fillScreen(TFT_BLACK);
  tft.setTextFont(2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  // Reset cursor to the top-left corner
  tft.setCursor(0, 0);

  // Display header
  tft.println("---------------------------");
  tft.println("KY034 Auto Flashing LED");
  tft.println("---------------------------");

  // Display static labels
  tft.setCursor(0, 70); // position for button state label
  tft.print("LED State:");
}

// Function to update dynamic elements on the TFT screen
void updateDynamicElements() {
  // Update LED state
  tft.setCursor(0, 90); // position for sensor state value (below the label)
  tft.print("      ");  // clear previous value (6 spaces)
  tft.setCursor(0, 90); // reset cursor
  tft.print(ledState ? "ON " : "OFF");
}


/*************************************************************
*********************** MAIN FUNCTIONS ***********************
**************************************************************/

// SETUP
void setup() {
  // Initialize the LED pin
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW); // turn off the LED initially

  // Initialize the backlight pin
  pinMode(PIN_LCD_BL, OUTPUT);
  digitalWrite(PIN_LCD_BL, HIGH); // turn on the backlight

  // Initialize the button pins (active low)
  pinMode(KEY_BUTTON, INPUT_PULLUP);
  pinMode(BOOT_BUTTON, INPUT_PULLUP);

  // TFT_eSPI setup
  tft.init();
  tft.setRotation(0);                     // adjust rotation (0 & 2 portrait | 1 & 3 landscape)
  tft.fillScreen(TFT_BLACK);              // black screen background
  tft.setTextFont(2);                     // set the font size
  tft.setTextColor(TFT_WHITE, TFT_BLACK); // set text colour (white) and background colour (black)

  // Draw static elements once
  drawStaticElements();

  // Initial display update
  redrawRequired = true; // update display
}

// MAIN LOOP
void loop() {
  // Read the current state of the BOOT button
  bool currentBootButtonState = digitalRead(BOOT_BUTTON);

  // Check if the BOOT button state has changed
  if (currentBootButtonState != lastBootButtonState) {
    // BOOT button is pressed (active LOW)
    if (currentBootButtonState == LOW) {
      ledState = !ledState;                         // toggle the LED state
      bootToggled = !bootToggled;                   // set the flag to indicate BOOT button has toggled the state
      digitalWrite(LED_PIN, ledState ? HIGH : LOW); // update the LED
      redrawRequired = true;                        // update the display
    }

    // Save the current BOOT button state for the next loop iteration
    lastBootButtonState = currentBootButtonState;
  }

  // Read the current state of the KEY button
  bool currentKeyButtonState = digitalRead(KEY_BUTTON);

  // Check if the KEY button state has changed
  if (currentKeyButtonState != lastKeyButtonState) {
    // KEY button is pressed (active LOW)
    if (currentKeyButtonState == LOW) {
      // Only turn on the LED if the BOOT button has not toggled the state
      if (!bootToggled) {
        digitalWrite(LED_PIN, HIGH); // power the KY-034 module
        ledState = true;             // update the LED state
        redrawRequired = true;       // update the display
      }
    }
    // KEY button is released
    else {
      // Only turn off the LED if the BOOT button has not toggled the state
      if (!bootToggled) {
        digitalWrite(LED_PIN, LOW); // cut power to the KY-034 module
        ledState = false;           // update the LED state
        redrawRequired = true;      // update the display
      }
    }

    // Save the current KEY button state for the next loop iteration
    lastKeyButtonState = currentKeyButtonState;
  }

  // Update the display if there is a change
  if (redrawRequired) {
    updateDynamicElements();
    redrawRequired = false; // reset the flag
  }

  delay(1); // slow down the loop to save CPU cycles
}
