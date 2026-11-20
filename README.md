# CuteEyes Robot 

An interactive, standalone robotic eye system that tracks human faces using an ESP32-S3-CAM and an ESP32-C3 OLED display, communicating wirelessly without the need for a laptop or internet connection.

##  Project Architecture
1. **ESP32-C3 (Eyes):** Creates a local Wi-Fi Access Point named `CuteEyes`. It receives coordinates via UDP and draws animated eyes on the OLED display.
2. **ESP32-S3-CAM (Brain):** Connects to the `CuteEyes` network. It captures video, detects faces using the ESP-DL library (`EloquentEsp32cam`), and sends the face coordinates via UDP to the C3 board.
3. **Standalone:** Both boards are powered by external batteries (Power Banks), making the robot fully independent.

##  Hardware Requirements
*   1x **ESP32-S3-CAM** (with OV2640 camera).
*   1x **ESP32-C3** development board with an integrated 72x40 SSD1306 OLED display.
*   2x USB Power Banks (or any 5V power source).
*   *No jumper wires are required between the two boards.*

##  Software & Libraries
*   **Arduino IDE**
*   **ESP32 Board Package** (Version 2.0.17 or 3.0.7 recommended).
*   **U8g2** (Display library).
*   **EloquentEsp32cam** (Face detection library).

##  Arduino IDE Settings

### For ESP32-S3-CAM:
*   **Board:** `ESP32S3 Dev Module Octal (WROOM2)` (or `ESP32S3 Dev Module`)
*   **Flash Size:** `16MB (128Mb)`
*   **Partition Scheme:** `Huge APP (3MB No OTA/1MB SPIFFS)`
*   **PSRAM:** `OPI PSRAM`
*   **USB CDC On Boot:** `Enabled`

### For ESP32-C3:
*   **Board:** `ESP32C3 Dev Module`
*   **Port:** Select the specific COM port for the C3.

##  How to Run
1. Open `ESP32-CAM.ino` (or the provided Camera code) and upload it to the **ESP32-S3-CAM**.
2. Open `ESP32-C3_Eyes.ino` (or the provided Eyes code) and upload it to the **ESP32-C3**.
3. Disconnect both boards from the computer.
4. Connect both boards to separate Power Banks.
5. The ESP32-C3 will create the `CuteEyes` Wi-Fi network.
6. The ESP32-S3-CAM will connect to it automatically and start tracking your face!

##  Troubleshooting
*   **Eyes move in the opposite direction?** 
    In the Camera code, swap the sign or reverse the math in `send_x` and `send_y` (e.g., change `face_cx - 120` to `120 - face_cx`).
*   **Camera not connecting?** 
    Ensure the ESP32-C3 is powered on *first* so the `CuteEyes` network is broadcasting.
*   **Face detection is slow?**
    This is normal. The ESP32-S3 processor is powerful but small. You can adjust `detection.fast()` in the Camera code, or reduce `camera.quality` to `camera.quality.low()`.
