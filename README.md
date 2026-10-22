# Smart Plant Monitoring & Precision Irrigation System

An autonomous Internet of Things (IoT) precision plant monitoring and closed-loop irrigation system. Designed for **ESP32** and **NodeMCU ESP8266** platforms, featuring multi-sensor environmental telemetry, intelligent hysteresis-based watering, hardware safety protections, and an interactive web interface.

---

## 📌 Project Overview

Traditional plant watering relies on manual estimation or static timers. Manual watering leads to human error (under-watering or root rot from over-watering), while timer-based systems waste significant water by running even when the soil is already saturated.

This project implements an **autonomous closed-loop IoT irrigation system** that:
1. Continuously monitors soil moisture, ambient temperature, relative humidity, and water reservoir levels.
2. Automatically triggers water delivery only when substrate moisture falls below calibrated thresholds.
3. Incorporates **dual-threshold hysteresis control** to eliminate relay chattering and mechanical wear.
4. Includes **dry-run safety protection** to prevent motor burnout if the water tank is depleted.
5. Provides an interactive live web dashboard and dedicated hardware workbench for real-time monitoring and control.

---

## ✨ Key System Features

- **Automated Precision Irrigation:** Uses dual-threshold hysteresis (`ON < 30%`, `OFF ≥ 65%`) to maintain optimal soil moisture without rapid on/off switching.
- **Corrosion-Free Capacitive Sensing:** Utilizes a capacitive soil moisture sensor with insulated solder-mask traces rather than resistive probes, preventing copper electrolysis in wet soil.
- **Local & Remote Telemetry Readouts:** 
  - **Local:** Real-time 0.96" I2C OLED display (SSD1306) mounted on the device.
  - **Remote:** Clean, mobile-responsive web dashboard with live status indicators.
- **Hardware Dry-Run & Timeout Protection:**
  - Water reservoir float switch inhibits pump operation when the tank is empty.
  - Software watchdog limits continuous pumping to a maximum of 15 seconds to prevent overflow.
- **Dynamic Plant Health Index (PHI):** Algorithmic multi-factor health score (0–100%) calculated from moisture, temperature, and humidity conditions.

---

## 📁 Repository Structure

```
iot-project/
├── index.html                   # Live Telemetry & Simulation Demo Dashboard
├── hardware.html                # Interactive Hardware & Circuit Workbench
├── README.md                    # Main Project Documentation
├── README.txt                   # Quickstart Text Reference
├── firmware/
│   ├── smart_plant_esp32.ino    # ESP32 C++ Arduino Sketch (OLED + DHT22 + Relay)
│   ├── smart_plant_nodemcu.ino  # NodeMCU ESP8266 C++ Arduino Sketch
│   ├── diagram.json             # Wokwi simulation circuit configuration
│   └── wokwi.toml               # Wokwi project config file
└── docs/
    ├── PROJECT_REPORT.md        # Comprehensive Engineering Project Report
    ├── TECHNICAL_FAQ.md         # Technical Architecture & Design FAQ
    └── CIRCUIT_DIAGRAM.md       # Pinout tables, wiring schematics & Bill of Materials
```

---

## 🚀 How to Run the Project

### 1. Live Web Dashboard (`index.html`)
1. Open [`index.html`](file:///Users/swasthik/Developer/01%20Projects/College/iot-project/iot-project/index.html) in any modern web browser (Chrome, Edge, Safari, Firefox).
2. **Interactive Demo:**
   - Click **`▶ Run 1-Click Automated Demo Cycle`** to demonstrate a full automated cycle (soil drying down, relay triggering, water delivery, and automated cutoff at target saturation).
   - Click **`🌵 Simulate Dry Soil`** or **`🌧️ Add Water`** to test individual threshold events.
   - Click **`🪣 Reservoir: Full / Empty`** to demonstrate the dry-run safety lockout.

### 2. Hardware & Circuit Workbench (`hardware.html`)
1. Open [`hardware.html`](file:///Users/swasthik/Developer/01%20Projects/College/iot-project/iot-project/hardware.html) via the top navigation bar.
2. Inspect the virtual ESP32 board, live OLED screen mirroring, and optocoupler relay module.
3. Click **`⚡ Test Relay Click`** to test the mechanical switching and audible feedback.
4. Use the **Live UART Serial Console** (115200 baud) to inspect raw sensor data and send serial commands (`D`, `W`, `P`).

### 3. Wokwi Browser Simulation (Optional)
1. Visit [Wokwi.com](https://wokwi.com) and start a new **ESP32** project.
2. Copy [`firmware/smart_plant_esp32.ino`](file:///Users/swasthik/Developer/01%20Projects/College/iot-project/iot-project/firmware/smart_plant_esp32.ino) into `sketch.ino`.
3. Paste [`firmware/diagram.json`](file:///Users/swasthik/Developer/01%20Projects/College/iot-project/iot-project/firmware/diagram.json) into the `diagram.json` tab.
4. Click **Play** to run the virtual ESP32 with OLED, DHT22, potentiometer, and relay.

---

## 🔌 Hardware Wiring & Pin Configuration

| Component | ESP32 Pin | NodeMCU Pin | Signal Type | Function |
| :--- | :--- | :--- | :--- | :--- |
| **Capacitive Soil Sensor** | `GPIO 34` | `A0 (ADC0)` | Analog Input | Measures soil moisture voltage (0–3.3V) |
| **5V Relay Module (Pump)** | `GPIO 23` | `D1 (GPIO 5)` | Digital Output | Active-LOW power drive for DC pump |
| **DHT22 Climate Sensor** | `GPIO 15` | `D2 (GPIO 4)` | Digital 1-Wire | Reads ambient temperature & humidity |
| **SSD1306 OLED (0.96")** | `GPIO 21` / `22` | `D2` / `D1` | I2C (SDA/SCL) | Local real-time telemetry display |
| **Reservoir Float Switch** | `GPIO 19` | `D6 (GPIO 12)` | Digital Input | Internal pull-up dry-run safety cutoff |
| **Status Indicator LEDs** | `GPIO 4` / `2` | `D7` / `D8` | Digital Output | Visual status & Wi-Fi connection indicator |

---

## 👥 Project Team & Contributions

- **Team Member 1:** Embedded Firmware, Hysteresis Irrigation Logic & ADC Calibration
- **Team Member 2:** Circuit Schematics, Electrical Protection & Wokwi Simulation
- **Team Member 3:** Front-End Web Dashboard, Telemetry Visualization & Event Handling
- **Team Member 4:** System Architecture Documentation, Edge-Case Testing & Project Report
