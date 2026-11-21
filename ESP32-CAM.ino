#include <eloquent_esp32cam.h>
#include <eloquent_esp32cam/face/detection.h>

using eloq::camera;
using eloq::face_t;
using eloq::face::detection;

// Updated UART Pins based on board labels (U0T and U0R)
#define TX_PIN 1
#define RX_PIN 3

void setup() {
  Serial.begin(115200); // For USB debugging
  delay(2000);

  // Initialize Serial1 to send data to ESP32-C3
  Serial1.begin(115200, SERIAL_8N1, RX_PIN, TX_PIN);

  // 1. Camera Setup
  camera.pinout.freenove_s3(); 
  camera.brownout.disable();
  
  // Face detection requires 240x240 resolution
  camera.resolution.face(); 
  camera.quality.high();

  // 2. Face Detection Setup
  detection.fast(); 
  detection.confidence(0.6); 

  // 3. Initialize Camera
  while (!camera.begin().isOk()) {
    Serial.println(camera.exception.toString());
    delay(1000);
  }
  Serial.println("Camera Ready!");
}

void loop() {
  // Capture frame
  if (!camera.capture().isOk()) return;
  
  // Run detection
  if (!detection.run().isOk()) return;

  int send_x = 0;
  int send_y = 0;

  if (detection.count() > 0) {
    face_t face = detection.first;
    
    // Calculate offset from center (240x240 -> center is 120)
    int offset_x = face.x - 120;
    int offset_y = face.y - 120;

    // Map to -100 to 100 range
    send_x = (offset_x * 100) / 120;
    send_y = (offset_y * 100) / 120;

    // Ensure values stay within bounds
    send_x = constrain(send_x, -100, 100);
    send_y = constrain(send_y, -100, 100);
  }

  // Send coordinates via physical wire (Serial1)
  Serial1.printf("%d,%d\n", send_x, send_y);
  
  delay(50); 
}