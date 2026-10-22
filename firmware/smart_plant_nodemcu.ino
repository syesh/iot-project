/*
 * Smart Plant Monitoring & Precision Irrigation System
 * Target Platform: NodeMCU ESP8266 (ESP-12E Module)
 * 
 * Sensors:
 *  - Capacitive Soil Moisture Sensor (Analog: A0)
 *  - DHT11 / DHT22 Temperature & Humidity Sensor (Digital: D2 / GPIO4)
 *  - LDR Light Sensor (or Digital Light Switch: D5 / GPIO14)
 *  - Ultrasonic / Water Level Probe (Digital: D6 / GPIO12)
 * Actuators:
 *  - 5V Relay Module for Submersible Water Pump (Digital: D1 / GPIO5 - Active LOW)
 *  - Status LED / Grow Light (Digital: D7 / GPIO13)
 *  - Buzzer for Low Reservoir Warning (Digital: D8 / GPIO15)
 *  - Optional I2C 16x2 LCD or SSD1306 OLED (SCL: D1, SDA: D2)
 *
 * Communication:
 *  - WiFi (802.11 b/g/n)
 *  - HTTP / MQTT Telemetry Publish & Remote Control
 */

#include <ESP8266WiFi.h>

// WiFi Configuration (Set your network credentials)
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// Hardware Pin Definitions
#define PIN_SOIL_ANALOG      A0     // NodeMCU ADC0 (0 - 1023)
#define PIN_RELAY_PUMP       D1     // GPIO5 - Controls 5V Water Pump Relay
#define PIN_DHT_DATA         D2     // GPIO4 - DHT11/DHT22 Data pin
#define PIN_WATER_LEVEL      D6     // GPIO12 - Reservoir water level switch
#define PIN_LED_STATUS       D7     // GPIO13 - System indicator / Grow light
#define PIN_BUZZER           D8     // GPIO15 - Low water alert buzzer

// Calibration constants for Soil Moisture Sensor (Analog reading 0 - 1023)
// In air (dry): ~850-1024, In water (wet): ~350-450
const int DRY_SENSOR_VALUE = 850;
const int WET_SENSOR_VALUE = 400;

// Threshold values (Hysteresis control)
int dryThreshold = 30;      // Turn pump ON if moisture falls below 30%
int targetMoisture = 65;    // Turn pump OFF when moisture reaches 65%

// State Variables
int currentMoisture = 55;
float currentTemp = 24.5;
float currentHumidity = 60.0;
bool isPumpActive = false;
bool isWaterAvailable = true;
bool autoMode = true;

// Timing Control (Non-blocking using millis)
unsigned long lastSensorReadTime = 0;
const unsigned long SENSOR_INTERVAL = 2000;    // Read sensors every 2s
unsigned long pumpStartTime = 0;
const unsigned long MAX_PUMP_RUN_TIME = 15000; // Safety cutoff: 15s max continuous pumping

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println();
  Serial.println(F("=================================================="));
  Serial.println(F("   SMART PLANT MONITORING & IRRIGATION SYSTEM     "));
  Serial.println(F("      NodeMCU ESP8266 Firmware v2.1               "));
  Serial.println(F("=================================================="));

  // Configure Pins
  pinMode(PIN_RELAY_PUMP, OUTPUT);
  pinMode(PIN_LED_STATUS, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_WATER_LEVEL, INPUT_PULLUP);

  // Relay modules are typically Active LOW: HIGH = OFF, LOW = ON
  digitalWrite(PIN_RELAY_PUMP, HIGH);
  digitalWrite(PIN_LED_STATUS, LOW);
  digitalWrite(PIN_BUZZER, LOW);

  // Connect to WiFi
  connectWiFi();
}

void loop() {
  unsigned long now = millis();

  // Read sensors periodically
  if (now - lastSensorReadTime >= SENSOR_INTERVAL) {
    lastSensorReadTime = now;
    readSensors();
    evaluateIrrigationLogic();
    printTelemetry();
  }

  // Safety Timer: Prevent pump burnout or reservoir flooding
  if (isPumpActive && (now - pumpStartTime >= MAX_PUMP_RUN_TIME)) {
    Serial.println(F("[SAFETY WARNING] Max pump runtime reached (15s). Forcing pump OFF."));
    setPump(false, "Emergency safety timeout");
  }

  // Process incoming Serial commands for hardware testing / simulation
  handleSerialCommands();
}

// Read Analog Soil Moisture and compute calibrated percentage
void readSensors() {
  int rawAnalog = analogRead(PIN_SOIL_ANALOG);
  
  // Map raw analog reading to 0 - 100%
  // Invert because higher analog reading typically means drier soil on resistive/capacitive modules
  int mappedMoisture = map(rawAnalog, DRY_SENSOR_VALUE, WET_SENSOR_VALUE, 0, 100);
  currentMoisture = constrain(mappedMoisture, 0, 100);

  // Read reservoir level sensor (LOW = water present with pullup)
  isWaterAvailable = (digitalRead(PIN_WATER_LEVEL) == LOW);

  // Simulated ambient readings (or DHT.readTemperature() / DHT.readHumidity() if library included)
  currentTemp = 24.0 + (random(-10, 15) / 10.0);
  currentHumidity = 55.0 + (random(-20, 20) / 10.0);
}

// Hysteresis & Closed-Loop Feedback Control Logic
void evaluateIrrigationLogic() {
  if (!autoMode) return;

  // Safety check: Do not run pump if reservoir is dry
  if (!isWaterAvailable) {
    if (isPumpActive) {
      setPump(false, "Low water reservoir safety lockout");
    }
    digitalWrite(PIN_BUZZER, HIGH);
    delay(100);
    digitalWrite(PIN_BUZZER, LOW);
    return;
  }

  // Case 1: Moisture is below dry threshold -> Activate pump
  if (currentMoisture < dryThreshold && !isPumpActive) {
    setPump(true, "Moisture below threshold");
  }

  // Case 2: Moisture reached or exceeded target moisture -> Deactivate pump
  if (currentMoisture >= targetMoisture && isPumpActive) {
    setPump(false, "Target moisture satisfied");
  }
}

// Actuator Controller
void setPump(bool state, const char* reason) {
  isPumpActive = state;
  if (state) {
    digitalWrite(PIN_RELAY_PUMP, LOW);  // Active LOW relay ON
    digitalWrite(PIN_LED_STATUS, HIGH); // Status LED ON
    pumpStartTime = millis();
    Serial.print(F("[ACTUATOR] Pump turned ON -> Reason: "));
  } else {
    digitalWrite(PIN_RELAY_PUMP, HIGH); // Active LOW relay OFF
    digitalWrite(PIN_LED_STATUS, LOW);  // Status LED OFF
    Serial.print(F("[ACTUATOR] Pump turned OFF -> Reason: "));
  }
  Serial.println(reason);
}

// WiFi Connection Handler
void connectWiFi() {
  Serial.print(F("Connecting to WiFi: "));
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 15) {
    delay(500);
    Serial.print(F("."));
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println();
    Serial.print(F("WiFi Connected! IP Address: "));
    Serial.println(WiFi.localIP());
  } else {
    Serial.println();
    Serial.println(F("WiFi connection timeout. Operating in Standalone/Offline mode."));
  }
}

// Telemetry output in JSON-compatible format for IoT dashboards
void printTelemetry() {
  Serial.print(F("{\"moisture\":"));
  Serial.print(currentMoisture);
  Serial.print(F(",\"temperature\":"));
  Serial.print(currentTemp, 1);
  Serial.print(F(",\"humidity\":"));
  Serial.print(currentHumidity, 1);
  Serial.print(F(",\"pump\":"));
  Serial.print(isPumpActive ? "1" : "0");
  Serial.print(F(",\"water_level\":"));
  Serial.print(isWaterAvailable ? "\"OK\"" : "\"LOW\"");
  Serial.println(F("}"));
}

// Handle test commands via Serial monitor
void handleSerialCommands() {
  if (Serial.available() > 0) {
    char cmd = Serial.read();
    if (cmd == 'P' || cmd == 'p') {
      setPump(!isPumpActive, "Manual Serial Toggle");
    } else if (cmd == 'A' || cmd == 'a') {
      autoMode = !autoMode;
      Serial.print(F("[MODE] Auto Mode set to: "));
      Serial.println(autoMode ? "ENABLED" : "DISABLED");
    } else if (cmd == 'D' || cmd == 'd') {
      // Simulate dry soil
      currentMoisture = 20;
      Serial.println(F("[TEST] Simulated Dry Soil (20%)"));
      evaluateIrrigationLogic();
    } else if (cmd == 'W' || cmd == 'w') {
      // Simulate wet soil
      currentMoisture = 75;
      Serial.println(F("[TEST] Simulated Wet Soil (75%)"));
      evaluateIrrigationLogic();
    }
  }
}
