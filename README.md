# Smart Plant Monitoring & Precision Irrigation System (IoT Mini Project)

[![IoT](https://img.shields.io/badge/IoT-ESP8266%20%7C%20ESP32-blue.svg)](https://espressif.com)
[![Simulation](https://img.shields.io/badge/Simulation-Wokwi%20%2B%20HTML5-success.svg)](https://wokwi.com)
[![Course](https://img.shields.io/badge/Rubric-Part%20B%20(10%20Marks)-orange.svg)](#rubric-coverage)

An autonomous Internet of Things (IoT) precision plant monitoring and closed-loop irrigation system. Designed for both **NodeMCU ESP8266** and **ESP32** platforms, complete with multi-sensor telemetry, dual-threshold hysteresis control, hardware safety lockouts, online Wokwi circuit simulation, and a web telemetry dashboard.

---

## 🎯 Evaluation Rubric Alignment (10 Marks Total)

| Evaluation Parameter | Marks | Project Implementation Details |
| :--- | :---: | :--- |
| **1. Project Idea & Innovation** | **2 Marks** | Addresses domestic and agricultural water wastage with intelligent closed-loop automation, dynamic **Plant Health Index (PHI)**, multi-parameter environmental sensing (Moisture, Temp, Humidity, Tank level), and customizable **Crop Presets** (Succulent, Houseplant, Tomato, Fern). |
| **2. Technical Implementation** | **5 Marks** | Full Arduino C++ firmware for **NodeMCU ESP8266** (`firmware/smart_plant_nodemcu.ino`) and **ESP32** (`firmware/smart_plant_esp32.ino`), verified **Wokwi** simulation (`diagram.json`, `wokwi.toml`), I2C SSD1306 OLED display driver, relay flyback protection design, non-blocking asynchronous timers (`millis()`), and dry-run safety lockouts. |
| **3. Team Collaboration & Documentation** | **3 Marks** | Complete academic report (`docs/PROJECT_REPORT.md`) featuring a dedicated **Team Contribution Matrix**, circuit schematics (`docs/CIRCUIT_DIAGRAM.md`), mathematical formulations, and a technical FAQ guide (`docs/TECHNICAL_FAQ.md`). |

---

## 📁 Repository Directory Structure

```
iot-project/
├── index.html                   # Clean, Simple Live Demo & Telemetry Dashboard
├── hardware.html                # Dedicated Hardware & Circuit Workbench
├── README.md                    # Main Project Guide & Submission Documentation
├── README.txt                   # Original quick reference text file
├── firmware/
│   ├── smart_plant_nodemcu.ino  # NodeMCU ESP8266 Arduino Sketch
│   ├── smart_plant_esp32.ino    # ESP32 Arduino Sketch (with OLED & DHT22)
│   ├── diagram.json             # Wokwi simulation circuit layout & wiring
│   └── wokwi.toml               # Wokwi configuration file
└── docs/
    ├── PROJECT_REPORT.md        # University-Grade Project Report (Rubric Aligned)
    ├── TECHNICAL_FAQ.md         # 10 Technical & Architectural Q&As
    └── CIRCUIT_DIAGRAM.md       # Pinout tables, electrical schematics, and BOM
```

---

## 🚀 How to Run & Demonstrate (No Hardware Required)

### 1. Simple Web Demo & Dashboard
1. Open [`index.html`](file:///Users/swasthik/Developer/01%20Projects/College/iot-project/iot-project/index.html) in any browser.
2. Click **`▶ Run 1-Click Automated Demo Cycle`** to show the complete self-watering cycle smoothly.
3. Switch over to [`hardware.html`](file:///Users/swasthik/Developer/01%20Projects/College/iot-project/iot-project/hardware.html) using the top navigation pill to show the virtual ESP32 board, OLED screen, clicking relay module, and UART serial console.
2. **Key Interactive Features to Demonstrate:**
   - **Auto Demo Mode:** Click `▶ Run Auto Demo` in the top right. Watch the system simulate soil drying, automatically trip the relay when falling below the dry threshold, show water spray animation, refill moisture, and automatically turn off at target saturation.
   - **Crop Presets:** Switch between *Desert Succulent*, *Houseplant*, *Vegetable*, and *Tropical Fern* to show how hysteresis thresholds dynamically adapt per crop.
   - **Dry-Run Safety Lockout:** Click `🪣 Tank: Full (OK)` to toggle the tank to *EMPTY*. Notice the pump immediately locks out with an alert sound, preventing motor burnout.
   - **Real-Time Rolling Chart:** Watch the live canvas chart plot real-time soil moisture and ambient temperature trends.
   - **Hardware & Code Inspector:** Click `⚡ Hardware & Code` in the top bar to display the circuit pinouts and copy the microcontroller code.

### 2. Microcontroller Simulation via Wokwi
1. Open [Wokwi.com](https://wokwi.com) in your browser and choose **ESP32**.
2. Copy the code from [`firmware/smart_plant_esp32.ino`](file:///Users/swasthik/Developer/01%20Projects/College/iot-project/iot-project/firmware/smart_plant_esp32.ino) into `sketch.ino`.
3. In Wokwi's `diagram.json` tab, paste the contents of [`firmware/diagram.json`](file:///Users/swasthik/Developer/01%20Projects/College/iot-project/iot-project/firmware/diagram.json).
4. Click **Play**:
   - The SSD1306 OLED displays live telemetry.
   - Rotate the **Soil Moisture potentiometer** to test automatic relay switching.
   - Slide DHT22 temperature/humidity sliders to see real-time updates.
   - Press the **Tank Float Switch** to test the dry-run buzzer alarm.

---

## ⚡ Hardware Pinout Summary

| Peripheral | NodeMCU ESP8266 Pin | ESP32 Pin | Description |
| :--- | :--- | :--- | :--- |
| **Soil Moisture Sensor** | `A0` (Analog ADC) | `GPIO 34` | Substrate dielectric measurement |
| **5V Relay Module (Pump)** | `D1` (GPIO 5) | `GPIO 23` | Active LOW switching for water pump |
| **DHT22 Climate Sensor** | `D2` (GPIO 4) | `GPIO 15` | Digital Temperature & Humidity |
| **Water Reservoir Float** | `D6` (GPIO 12) | `GPIO 19` | Internal pull-up safety float switch |
| **SSD1306 OLED Display** | `D1` (SCL) / `D2` (SDA) | `GPIO 22` / `GPIO 21` | I2C Telemetry Display (0x3C) |
| **Piezo Buzzer** | `D8` (GPIO 15) | `GPIO 18` | Acoustic alert indicator |

---

## 👥 Team Members & Contribution

Please fill in your names and USNs in [`docs/PROJECT_REPORT.md`](file:///Users/swasthik/Developer/01%20Projects/College/iot-project/iot-project/docs/PROJECT_REPORT.md):
- **Member 1:** Firmware logic, hysteresis algorithm, safety timers (`firmware/*.ino`)
- **Member 2:** Circuit schematics, BOM, and Wokwi simulation configuration (`diagram.json`)
- **Member 3:** IoT telemetry dashboard UI, HTML5 canvas charts, and crop engine (`index.html`)
- **Member 4:** Project report compilation, test validation, and technical documentation (`docs/*.md`)
