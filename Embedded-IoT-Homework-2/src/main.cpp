#include <Arduino.h>

// ---- Pins ----
const int LED1_PIN = 18;
const int LED2_PIN = 19;

// ---- LEDC channels ----
// ESP32 mein channel pairs timer share karte hain (0,1 -> timer0, 2,3 -> timer1),
// isliye alag frequencies ke liye channel 0 aur 2 use kiye hain.
const int LED1_CH = 0;
const int LED2_CH = 2;

// ---- PWM settings ----
const uint32_t LED1_FREQ = 1000;   // Hz
const uint32_t LED2_FREQ = 5000;   // Hz
const uint8_t  PWM_RES   = 10;     // bits -> duty 0..1023
const int      MAX_DUTY  = (1 << PWM_RES) - 1;

// ---- Duty values (Task 2 mein OLED par yehi dikhayenge) ----
int duty1 = 0;
int duty2 = MAX_DUTY;

int step1 = 5;
int step2 = -5;

void setup() {
  Serial.begin(115200);

  ledcSetup(LED1_CH, LED1_FREQ, PWM_RES);
  ledcAttachPin(LED1_PIN, LED1_CH);

  ledcSetup(LED2_CH, LED2_FREQ, PWM_RES);
  ledcAttachPin(LED2_PIN, LED2_CH);
}

void loop() {
  duty1 += step1;
  if (duty1 >= MAX_DUTY) { duty1 = MAX_DUTY; step1 = -step1; }
  if (duty1 <= 0)        { duty1 = 0;        step1 = -step1; }

  duty2 += step2;
  if (duty2 >= MAX_DUTY) { duty2 = MAX_DUTY; step2 = -step2; }
  if (duty2 <= 0)        { duty2 = 0;        step2 = -step2; }

  ledcWrite(LED1_CH, duty1);
  ledcWrite(LED2_CH, duty2);

  Serial.printf("LED1: %d%% (%luHz)  LED2: %d%% (%luHz)\n",
                duty1 * 100 / MAX_DUTY, (unsigned long)LED1_FREQ,
                duty2 * 100 / MAX_DUTY, (unsigned long)LED2_FREQ);

  delay(10);
}