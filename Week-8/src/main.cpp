#include <Arduino.h>

#define DAC_PIN 25  // DAC1

const int levels[] = {0, 64, 128, 192, 255};
const int numLevels = sizeof(levels) / sizeof(levels[0]);

void setup() {
  Serial.begin(115200);
  Serial.println("Program 1: Stepped DAC Output on GPIO25");
}

void loop() {
  for (int i = 0; i < numLevels; i++) {
    dacWrite(DAC_PIN, levels[i]);
    Serial.print("DAC value: ");
    Serial.print(levels[i]);
    Serial.print("  -> approx voltage: ");
    Serial.print(levels[i] * 3.3 / 255.0, 2);
    Serial.println(" V");
    delay(2000);  
  }
  Serial.println("---- cycle done ----");
}