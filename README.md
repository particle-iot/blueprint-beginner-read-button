# Blueprint – Beginner – Read Button & Toggle LED

**Repo:** `blueprint-beginner-read-button`  
**Difficulty:** Beginner  
**Estimated Time:** 15–30 minutes  
**Hardware Needed:** Push button (momentary), jumper wires (uses onboard LED)  
**Particle Features:** `GPIO (pinMode, digitalRead, digitalWrite)`, `Particle.publish()`, `Console`

---

## Overview
Use an externally connected push button to toggle the onboard LED and publish the button state whenever it changes.  
The button is connected to a GPIO with an **internal pull-up** enabled (active-LOW). If you have an **external pull-up**, this still works; just wire the button between the GPIO and GND.

---

## Wiring
- **Button → GPIO D2 and GND** (default configuration with `INPUT_PULLUP`)  
- **LED → Onboard LED (LED_BUILTIN or D7)**

> Using `INPUT_PULLUP` makes the input read **HIGH** when released and **LOW** when pressed.

---

## Tools & Materials
- Any Particle device (Photon 2, Boron, etc.)  
- USB cable  
- Push button (momentary) + jumper wires  
- [Particle Workbench](https://docs.particle.io/workbench/) or [Particle CLI](https://docs.particle.io/tutorials/developer-tools/cli/)  

---

## Steps
1. **Clone this repository:**
   ```bash
   git clone https://github.com/<your-org>/blueprint-beginner-read-button.git
   cd blueprint-beginner-read-button
   ```
2. **Open the project** in Particle Workbench or your preferred editor.  
3. **Wire the button** between `D2` and `GND` (or change `BUTTON_PIN` in code).  
4. **Flash the firmware** to your device:
   ```bash
   particle flash <device-name>
   ```
5. **Press the button** and observe:
   - LED toggles **on each press**  
   - **Console → Logs** shows a `button_state` event with `"PRESSED"` or `"RELEASED"`  
   - `led_state` event indicates the new LED state

---

## How It Works
- `pinMode(BUTTON_PIN, INPUT_PULLUP)` enables the internal pull-up resistor.  
- A simple software debounce accepts changes stable for 50 ms.  
- On a stable transition to **pressed (LOW)**, the firmware toggles the LED and publishes `led_state`.  
- On any stable change, it publishes `button_state` with `"PRESSED"` or `"RELEASED"`.

---

## Source Code
Main firmware: [`/src/read-button.cpp`](src/read-button.cpp).  
It’s fully commented to double as standalone documentation.

---

## Expected Output
**Console events:**
```
Event: button_state   Data: PRESSED
Event: led_state      Data: ON
Event: button_state   Data: RELEASED
```

**Serial logs (from `particle serial monitor --follow`):**
```
[app] INFO: Published button_state: PRESSED
[app] INFO: Published led_state: ON
[app] INFO: Published button_state: RELEASED
```

---

## Extensions
- Change `BUTTON_PIN` for your wiring.  
- Replace polling with an interrupt (`attachInterrupt`) plus a debounce timer.  
- Add a publish rate limit to prevent noisy hardware from flooding events.
