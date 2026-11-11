#include <Arduino.h>

#define DAC_PIN 25   // DAC1 (true analog voltage)
#define PWM_PIN 16   // PWM (digital switching)

#define PWM_CHANNEL 0
#define PWM_FREQ    5000  // 5 kHz
#define PWM_RES     8     // 8-bit (0-255)

const int FADE_DELAY = 20;  // ms

void setup() {
  Serial.begin(115200);
  Serial.println("Task 6: DAC (GPIO25) + PWM (GPIO16) simultaneously");

  ledcSetup(PWM_CHANNEL, PWM_FREQ, PWM_RES);
  ledcAttachPin(PWM_PIN, PWM_CHANNEL);
}

void loop() {
  // Fade UP
  for (int value = 0; value <= 255; value++) {
    dacWrite(DAC_PIN, value);
    ledcWrite(PWM_CHANNEL, value);
    delay(FADE_DELAY);
  }
  Serial.println("Fade UP done");

  // Fade DOWN
  for (int value = 255; value >= 0; value--) {
    dacWrite(DAC_PIN, value);
    ledcWrite(PWM_CHANNEL, value);
    delay(FADE_DELAY);
  }
  Serial.println("Fade DOWN done");
}