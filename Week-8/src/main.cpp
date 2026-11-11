#include <Arduino.h>

#define DAC_PIN 26  

const int FADE_DELAY = 20; 

void setup() {
  Serial.begin(115200);
  Serial.println("Program 3: DAC2 Fade on GPIO26");
}

void loop() {
  // Fade UP: 0 -> 255
  for (int value = 0; value <= 255; value++) {
    dacWrite(DAC_PIN, value);
    delay(FADE_DELAY);
  }
  Serial.println("Fade UP done");

  // Fade DOWN: 255 -> 0
  for (int value = 255; value >= 0; value--) {
    dacWrite(DAC_PIN, value);
    delay(FADE_DELAY);
  }
  Serial.println("Fade DOWN done");
}