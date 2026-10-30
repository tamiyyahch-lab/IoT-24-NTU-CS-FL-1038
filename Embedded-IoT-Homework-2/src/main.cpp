#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_SDA 21
#define OLED_SCL 22
#define OLED_ADDR 0x3C

#define LED1_PIN 18
#define LED2_PIN 19

#define LED1_CH 0
#define LED2_CH 2
#define LED1_FREQ 1000
#define LED2_FREQ 5000
#define PWM_RES 8
#define MAX_DUTY 255

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

int duty1 = 0;
int duty2 = 0;
int step1 = 2;
int step2 = 3;
unsigned long lastDisplay = 0;

int toPercent(int duty) {
  return duty * 100 / MAX_DUTY;
}

void fadeStep(int &duty, int &step) {
  duty = duty + step;
  if (duty >= MAX_DUTY) {
    duty = MAX_DUTY;
    step = -step;
  }
  if (duty <= 0) {
    duty = 0;
    step = -step;
  }
}

void updateDisplay() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 0);
  display.println("PWM MONITOR");
  display.drawLine(0, 12, 127, 12, SSD1306_WHITE);

  display.setCursor(0, 24);
  display.print("LED1: ");
  display.print(toPercent(duty1));
  display.print("% F:");
  display.print(LED1_FREQ);
  display.println("Hz");

  display.setCursor(0, 40);
  display.print("LED2: ");
  display.print(toPercent(duty2));
  display.print("% F:");
  display.print(LED2_FREQ);
  display.println("Hz");

  display.display();
}

void setup() {
  Serial.begin(115200);

  Wire.begin(OLED_SDA, OLED_SCL);
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println("OLED initialization failed!");
    while (true) {
      delay(100);
    }
  }

  ledcSetup(LED1_CH, LED1_FREQ, PWM_RES);
  ledcAttachPin(LED1_PIN, LED1_CH);
  ledcSetup(LED2_CH, LED2_FREQ, PWM_RES);
  ledcAttachPin(LED2_PIN, LED2_CH);

  updateDisplay();
}

void loop() {
  fadeStep(duty1, step1);
  fadeStep(duty2, step2);

  ledcWrite(LED1_CH, duty1);
  ledcWrite(LED2_CH, duty2);

  if (millis() - lastDisplay >= 100) {
    lastDisplay = millis();
    updateDisplay();

    Serial.print("LED1: ");
    Serial.print(toPercent(duty1));
    Serial.print("% (1000 Hz)  LED2: ");
    Serial.print(toPercent(duty2));
    Serial.println("% (5000 Hz)");
  }

  delay(15);
}