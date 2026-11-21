#include <U8g2lib.h>

// OLED 72x40 (SCL=6, SDA=5)
U8G2_SSD1306_72X40_ER_F_HW_I2C display(U8G2_R0, U8X8_PIN_NONE, 6, 5);

// UART Pins for receiving from ESP32-S3
#define RX_PIN 20
#define TX_PIN 21

int faceX = 0;
int faceY = 0;

// Blinking state variables
unsigned long lastBlink = 0;
unsigned long blinkStart = 0;
bool blinking = false;

// Idle movement variables
int idleOffsetX = 0;
int idleOffsetY = 0;
unsigned long lastIdle = 0;

void setup() {
  Serial.begin(115200); // For USB debugging
  // Initialize Serial1 to receive from ESP32-S3
  Serial1.begin(115200, SERIAL_8N1, RX_PIN, TX_PIN);

  display.begin();
  display.setContrast(255);

  randomSeed(esp_random());
  lastBlink = millis() + 2500;
  lastIdle = millis() + 3000;
}

void loop() {
  // Read incoming coordinates from the wire
  if (Serial1.available()) {
    String data = Serial1.readStringUntil('\n');
    data.trim();
    int comma = data.indexOf(',');
    if (comma > 0) {
      faceX = constrain(data.substring(0, comma).toInt(), -100, 100);
      faceY = constrain(data.substring(comma + 1).toInt(), -100, 100);
    }
  }

  unsigned long now = millis();

  // Natural blinking logic
  if (!blinking && now >= lastBlink) {
    blinking = true;
    blinkStart = now;
  }
  if (blinking && now - blinkStart >= 150) {
    blinking = false;
    lastBlink = now + random(2500, 5500);
  }

  // Idle movement logic
  if (now >= lastIdle) {
    idleOffsetX = random(-2, 3);
    idleOffsetY = random(-1, 2);
    lastIdle = now + random(2000, 4000);
  }

  // Map coordinates to screen movement
  int x = map(faceX, -100, 100, -5, 5);
  int y = map(faceY, -100, 100, -3, 3);

  // If no face detected, use idle movement
  if (abs(faceX) < 8 && abs(faceY) < 8) {
    x = idleOffsetX;
    y = idleOffsetY;
  }

  // Tall rounded rectangular eyes (Original design)
  const int eyeWidth = 13;
  const int eyeHeight = 19;
  const int radius = 4;

  int leftX = 19 + x;
  int rightX = 40 + x;
  int eyeY = 20 + y - eyeHeight / 2;

  display.clearBuffer();

  if (blinking) {
    // Draw flat lines when blinking
    display.drawHLine(leftX, 20 + y, eyeWidth);
    display.drawHLine(rightX, 20 + y, eyeWidth);
  } else {
    // Draw rounded boxes when open
    display.drawRBox(leftX, eyeY, eyeWidth, eyeHeight, radius);
    display.drawRBox(rightX, eyeY, eyeWidth, eyeHeight, radius);
  }

  display.sendBuffer();
  delay(20);
}