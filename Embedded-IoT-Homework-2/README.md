# Embedded IoT Homework 2: Dual-LED PWM Control with OLED Monitoring

This project fades two LEDs on an ESP32 using two different PWM frequencies. An SSD1306 I2C OLED shows the live duty cycle (in percent) and the configured PWM frequency of both LEDs. The project was built and tested in Wokwi and on a physical ESP32 hardware kit.

- Wokwi project link: https://wokwi.com/projects/479287557601501185
- Board: ESP32 DevKit
- Framework: Arduino, built with PlatformIO (`espressif32@6.9.0`, Arduino-ESP32 core 2.x)
- Libraries: Adafruit GFX Library, Adafruit SSD1306

## Task summary

| Task | Description | Status |
|------|-------------|--------|
| Task 1 | Two LEDs with separate resistors, fading with different PWM frequencies | Done (Wokwi and hardware) |
| Task 2 | SSD1306 OLED showing live duty cycle and frequency of both LEDs | Done (Wokwi and hardware) |

Commit history: `task1 completed` (Task 1), then `complete homework` (final project).

## Wiring

| Component | ESP32 pin |
|-----------|-----------|
| LED 1 (red), through its own 220 ohm resistor | GPIO 18 |
| LED 2 (green), through its own 220 ohm resistor | GPIO 19 |
| OLED SDA | GPIO 21 |
| OLED SCL | GPIO 22 |
| OLED VCC | 3V3 |
| OLED GND | GND |
| LED 1 and LED 2 cathodes (short legs) | GND |

Each LED path is: `GPIO -> 220 ohm resistor -> LED anode (long leg) -> LED cathode (short leg) -> GND`.

Every LED has its own current-limiting resistor. The OLED uses I2C address `0x3C` and is powered from 3.3 V.

## PWM configuration

| | LED 1 | LED 2 |
|---|---|---|
| GPIO | 18 | 19 |
| LEDC channel | 0 | 2 |
| PWM frequency | 1000 Hz | 5000 Hz |
| Resolution | 8 bit (duty value 0 to 255) | 8 bit (duty value 0 to 255) |
| Fade step per loop | 2 | 3 |

8 bit resolution gives 256 brightness levels, which is smooth enough for LED fading and works well at both 1000 Hz and 5000 Hz.

## How the fading works

The main loop runs every 15 ms. In each loop the duty value of each LED is increased (or decreased) by its step. When the duty reaches 255 the direction reverses, and when it reaches 0 it reverses again. So each LED fades from minimum to maximum brightness and back. LED 2 has a bigger step, so it fades faster than LED 1.

The duty values (`duty1`, `duty2`) are stored in global variables. The same variables are written to the LEDs with `ledcWrite()` and are used for the OLED text, so the display always matches the values the program is actually using. The percentage is calculated as `duty * 100 / 255`.

## LEDC timer sharing (technical check)

The ESP32 LEDC peripheral has only a few PWM timers, and several channels share each timer. Channels that share a timer must use the same frequency, so setting a second frequency on a channel of the same timer would change the first one.

On the original ESP32, channels 0 and 1 share one timer, and channels 2 and 3 share the next timer. To get two truly independent frequencies, LED 1 uses channel 0 and LED 2 uses channel 2, so they run on different timers. This is why the two LEDs can use 1000 Hz and 5000 Hz at the same time without affecting each other.

## Duty cycle and frequency

- Duty cycle is the percentage of one PWM period where the signal is HIGH. A higher duty cycle gives more average power to the LED, so it looks brighter. 0 percent is off and 100 percent is fully on.
- The eye cannot follow very fast switching, so it sees the average brightness, not the individual pulses.
- Frequency is the number of ON/OFF cycles per second. At 5000 Hz the signal switches faster than at 1000 Hz. Both are fast enough that the LED looks steady with no visible flicker. The frequency changes how fast the signal switches, not how bright the LED looks. The brightness depends on the duty cycle.

## OLED output

The OLED is refreshed every 100 ms. It shows:

```
PWM MONITOR
LED1: 25% F:1000Hz
LED2: 75% F:5000Hz
```

The percentages change while the LEDs fade. The frequencies stay at 1000 Hz and 5000 Hz. The Serial Monitor (115200 baud) prints the same percentages.

## Evidence

Wokwi screenshots and hardware photographs are in the `screenshots/` folder of this project.

## Test results

| Check | Wokwi | Hardware |
|-------|-------|----------|
| LED 1 fades from minimum to maximum and back | Pass | Pass |
| LED 2 fades from minimum to maximum and back | Pass | Pass |
| LED 1 and LED 2 use different frequencies (1000 Hz and 5000 Hz) | Pass | Pass |
| OLED shows duty cycle percentage of both LEDs | Pass | Pass |
| OLED shows configured frequency of both LEDs | Pass | Pass |
| OLED values change as the LEDs fade | Pass | Pass |

## Wokwi vs hardware

- In Wokwi the wiring is exact and the upload step does not exist, so it worked directly. On hardware, the wiring had to be checked pin by pin on the breadboard, and the code upload needed the correct USB cable and the BOOT button.
- On hardware, the OLED needed the correct I2C wiring (SDA on GPIO 21 and SCL on GPIO 22) and a 3.3 V supply. In Wokwi the display is always found.
- LED brightness looks different on real LEDs because of the resistor value and the LED type, while Wokwi shows an ideal LED.

## Project structure

```
Embedded-IoT-Homework-2/
├── diagram.json
├── wokwi.toml
├── platformio.ini
├── src/
│   └── main.cpp
├── screenshots/
│   ├── wokwi_circuit.png
│   ├── wokwi_running_oled.png
│   ├── hardware_circuit.jpg
│   └── hardware_oled_output.jpg
└── README.md
```

## How to build and run

1. Open the project folder in VS Code with PlatformIO.
2. Build with the PlatformIO Build button.
3. For Wokwi, start the simulation from `diagram.json`.
4. For hardware, connect the ESP32 by USB, upload the code, and open the Serial Monitor at 115200 baud.
