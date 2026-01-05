#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define I2C_ADDRESS 0x3C

#define SOUND_DO 2   // Sound sensor digital output
#define RED_LED 4    // Red LED pin

Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int alertDuration = 30; // seconds for red light

void setup() {
  Serial.begin(115200);

  if (!display.begin(I2C_ADDRESS, true)) {
    Serial.println("SH1106 not found");
    while (1);
  }

  pinMode(SOUND_DO, INPUT);
  pinMode(RED_LED, OUTPUT);

  display.clearDisplay();
  display.setTextColor(SH110X_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 10);
  display.println("! HOSPITAL ZONE !");
  display.setCursor(0, 30);
  display.println("No Horning!");
  display.display();

  digitalWrite(RED_LED, LOW); // make sure LED is off
  delay(4000); // show initial warning for 3 seconds
}

void loop() {
  bool soundDetected = digitalRead(SOUND_DO);

  if (soundDetected) {
    // TURN ON RED LIGHT
    digitalWrite(RED_LED, HIGH);

    // COUNTDOWN DISPLAY
    for (int i = alertDuration; i >= 0; i--) {
      display.clearDisplay();
      display.setTextSize(1);
      display.setCursor(0, 10);
      display.println("! HOSPITAL ZONE !");
      display.setCursor(0, 30);
      display.println("No Horning!Red Light");

      display.setTextSize(2);
      display.setCursor(40, 50);
      display.print(i);
      display.println(" s"); // seconds remaining
      display.display();

      delay(1000); // 1 second
    }

    // TURN OFF RED LIGHT AFTER COUNTDOWN
    digitalWrite(RED_LED, LOW);
  } else {
    // MONITORING MODE
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 10);
    display.println("! HOSPITAL ZONE !");
    display.setCursor(0, 30);
    display.println("Monitoring...");
    display.display();

    digitalWrite(RED_LED, LOW);
    delay(100); // refresh quickly
  }
}
