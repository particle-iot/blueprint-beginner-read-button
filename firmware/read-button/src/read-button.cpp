/*
 * Project: Read Button & Toggle LED
 * Repo: blueprint-beginner-read-button
 * Author: Particle
 * Date: 2025-05-04
 *
 * Description:
 *  Beginner blueprint showing how to read a push button on a GPIO using an internal pull-up
 *  (with optional external pull-up), debounce changes, toggle an LED on button press, and
 *  publish button state changes to the Particle Cloud.
 *
 *  Wiring (default with internal pull-up):
 *   - BUTTON between GPIO (default: D2) and GND
 *   - LED uses onboard LED (LED_BUILTIN or D7)
 *
 *  Behavior:
 *   - Button is active-LOW (pressed == LOW) when INPUT_PULLUP is used
 *   - On a stable transition to PRESSED, toggle the LED
 *   - On any stable change, publish "button_state" with "PRESSED" or "RELEASED"
 *   - Optionally also publish "led_state" after a toggle for visibility
 *
 * Docs:
 *   - pinMode / INPUT_PULLUP:
 *     https://docs.particle.io/reference/device-os/firmware/#pinmode-
 *   - digitalRead / digitalWrite:
 *     https://docs.particle.io/reference/device-os/firmware/#digitalread-
 *     https://docs.particle.io/reference/device-os/firmware/#digitalwrite-
 *   - Particle.publish():
 *     https://docs.particle.io/reference/device-os/firmware/#particle-publish-
 */

#include "Particle.h"

SYSTEM_MODE(AUTOMATIC);
SYSTEM_THREAD(ENABLED);
SerialLogHandler logHandler(LOG_LEVEL_INFO); // View with: particle serial monitor --follow

// -------------------------------------------------------------------
// Hardware configuration
// -------------------------------------------------------------------
// LED pin: prefer LED_BUILTIN if defined, otherwise D7
#ifndef LED_BUILTIN
  #define LED_PIN D7
#else
  #define LED_PIN LED_BUILTIN
#endif

// Button pin (change if you wired to a different GPIO)
#define BUTTON_PIN D2

// Debounce interval in milliseconds
constexpr uint32_t DEBOUNCE_MS = 50;

// State tracking
bool ledOn = false;
int lastStableButton = HIGH;     // INPUT_PULLUP default = HIGH (released)
int lastReadButton = HIGH;
uint32_t lastChangeMs = 0;

// Helpers to publish states
void publishButtonState(bool pressed) {
    Particle.publish("button_state", pressed ? "PRESSED" : "RELEASED", PRIVATE);
    Log.info("Published button_state: %s", pressed ? "PRESSED" : "RELEASED");
}
void publishLedState() {
    const char* state = ledOn ? "ON" : "OFF";
    Particle.publish("led_state", state, PRIVATE);
    Log.info("Published led_state: %s", state);
}

// -------------------------------------------------------------------
// setup() — runs once
// -------------------------------------------------------------------
void setup() {
    // LED output
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);
    ledOn = false;

    // Button input with internal pull-up enabled (active-LOW)
    // If you have an external pull-up, this still works; if you have an external pull-down,
    // change to INPUT and invert logic accordingly.
    pinMode(BUTTON_PIN, INPUT_PULLUP);

    // Initialize state
    lastStableButton = digitalRead(BUTTON_PIN);
    lastReadButton = lastStableButton;
    lastChangeMs = millis();

    Log.info("Setup complete. Using BUTTON_PIN=%d, LED_PIN=%d", BUTTON_PIN, LED_PIN);
    Log.info("Initial button (HIGH=released, LOW=pressed): %d", lastStableButton);
    publishLedState();
}

// -------------------------------------------------------------------
// loop() — debounce + state change detection
// -------------------------------------------------------------------
void loop() {
    int reading = digitalRead(BUTTON_PIN);

    if (reading != lastReadButton) {
        // Input changed; reset debounce timer
        lastReadButton = reading;
        lastChangeMs = millis();
    }

    // If the input has been stable for DEBOUNCE_MS, accept it
    if (millis() - lastChangeMs >= DEBOUNCE_MS) {
        if (reading != lastStableButton) {
            // Stable transition detected
            lastStableButton = reading;
            bool pressed = (lastStableButton == LOW); // active-LOW

            // Publish button state change
            publishButtonState(pressed);

            // Toggle LED on press (not on release) to avoid double-toggles
            if (pressed) {
                ledOn = !ledOn;
                digitalWrite(LED_PIN, ledOn ? HIGH : LOW);
                publishLedState();
            }
        }
    }

    delay(5); // keep loop light & OTA responsive
}
