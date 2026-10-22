/*
 * Smart Plant Monitoring & Precision Irrigation System
 * Target Platform: ESP32 (Wokwi & Hardware Compatible)
 * 
 * Features:
 *  - Analog Soil Moisture Sensing (GPIO 34 / ADC1)
 *  - DHT22 Digital Temperature & Humidity Sensor (GPIO 15)
 *  - SSD1306 0.96" I2C OLED Display (SDA: GPIO 21, SCL: GPIO 22)
 *  - 5V Relay module for Water Pump (GPIO 23)
 *  - Status LEDs for Network & Pump state (GPIO 2 & GPIO 4)
 *  - Piezo Buzzer for Alerts (GPIO 18)
 *  - Reservoir Float Switch (GPIO 19)
 */

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Pin Definitions
#define PIN_SOIL_ADC     34
#define PIN_DHT          15
#define PIN_RELAY_PUMP   23
#define PIN_LED_PUMP     4
#define PIN_LED_SYSTEM   2
#define PIN_BUZZER       18
#define PIN_WATER_FLOAT  19

#define DHTTYPE DHT22
DHT dht(PIN_DHT, DHTTYPE);

// Thresholds
const int DRY_THRESHOLD = 30;     // %
const int TARGET_MOISTURE = 65;   // %
const unsigned long MAX_PUMP_RUN_MS = 10000; // 10 second safety cutoff

// Variables
int moisturePercent = 55;
float temperature = 24.5;
float humidity = 60.0;
bool pumpState = false;
bool reservoirOk = true;
unsigned long pumpStartMs = 0;
unsigned long lastUpdateMs = 0;

void setup() {
  Serial.begin(115200);
  delay(200);

  pinMode(PIN_RELAY_PUMP, OUTPUT);
  pinMode(PIN_LED_PUMP, OUTPUT);
  pinMode(PIN_LED_SYSTEM, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_WATER_FLOAT, INPUT_PULLUP);

  digitalWrite(PIN_RELAY_PUMP, LOW);
  digitalWrite(PIN_LED_PUMP, LOW);
  digitalWrite(PIN_LED_SYSTEM, HIGH); // System power indicator

  dht.begin();

  // Initialize OLED display (Address 0x3C is standard for SSD1306)
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("[WARN] SSD1306 allocation failed or not connected"));
  } else {
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    display.setCursor(10, 20);
    display.println(F("SMART PLANT IOT"));
    display.setCursor(10, 36);
    display.println(F("Initializing..."));
    display.display();
    delay(1000);
  }

  Serial.println(F("=== SMART PLANT MONITORING SYSTEM ONLINE ==="));
}

void loop() {
  unsigned long now = millis();

  // Read Telemetry every 2 seconds
  if (now - lastUpdateMs >= 2000) {
    lastUpdateMs = now;
    readSensors();
    processIrrigation();
    updateOledDisplay();
    outputSerialJson();
  }

  // Safety Timeout for Pump
  if (pumpState && (now - pumpStartMs >= MAX_PUMP_RUN_MS)) {
    setPump(false, "Safety cutoff - max runtime exceeded");
  }

  // Process incoming commands from Serial monitor
  if (Serial.available()) {
    char c = Serial.read();
    if (c == 'p' || c == 'P') setPump(!pumpState, "Serial manual toggle");
  }
}

void readSensors() {
  // Read Potentiometer / Analog Soil Sensor (ESP32 ADC is 12-bit: 0 - 4095)
  int rawAdc = analogRead(PIN_SOIL_ADC);
  // Map ADC reading to percentage: 4095 (dry) -> 0%, 1500 (wet) -> 100%
  // Or in Wokwi potentiometer: 0 -> 0%, 4095 -> 100%
  moisturePercent = map(rawAdc, 0, 4095, 0, 100);
  moisturePercent = constrain(moisturePercent, 0, 100);

  // Read DHT22
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  if (!isnan(t)) temperature = t;
  if (!isnan(h)) humidity = h;

  // Read Water Reservoir Float switch (LOW = water present)
  reservoirOk = (digitalRead(PIN_WATER_FLOAT) == LOW);
}

void processIrrigation() {
  if (!reservoirOk) {
    if (pumpState) setPump(false, "Reservoir low - pump protection");
    tone(PIN_BUZZER, 1000, 200);
    return;
  }

  // Automatic Hysteresis Logic
  if (moisturePercent < DRY_THRESHOLD && !pumpState) {
    setPump(true, "Moisture below threshold (Dry)");
  } else if (moisturePercent >= TARGET_MOISTURE && pumpState) {
    setPump(false, "Target moisture reached (Optimal)");
  }
}

void setPump(bool on, const char* reason) {
  pumpState = on;
  digitalWrite(PIN_RELAY_PUMP, on ? HIGH : LOW);
  digitalWrite(PIN_LED_PUMP, on ? HIGH : LOW);

  if (on) {
    pumpStartMs = millis();
    tone(PIN_BUZZER, 2000, 100);
    Serial.print(F("[PUMP ACTIVATED] Reason: "));
  } else {
    Serial.print(F("[PUMP DEACTIVATED] Reason: "));
  }
  Serial.println(reason);
}

void updateOledDisplay() {
  display.clearDisplay();
  
  // Header bar
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print(F("SMART PLANT | ESP32"));
  display.drawLine(0, 10, 128, 10, SSD1306_WHITE);

  // Moisture metric
  display.setCursor(0, 16);
  display.print(F("Moist: "));
  display.setTextSize(2);
  display.print(moisturePercent);
  display.print(F("%"));

  // Status & Temp
  display.setTextSize(1);
  display.setCursor(0, 36);
  display.print(F("Temp: "));
  display.print(temperature, 1);
  display.print(F("C  H:"));
  display.print((int)humidity);
  display.print(F("%"));

  // Pump & Tank Status
  display.setCursor(0, 50);
  display.print(F("PUMP: "));
  display.print(pumpState ? "ON [RUNNING]" : "OFF [IDLE]");
  
  display.display();
}

void outputSerialJson() {
  Serial.print(F("{\"moisture\":"));
  Serial.print(moisturePercent);
  Serial.print(F(",\"temp\":"));
  Serial.print(temperature, 1);
  Serial.print(F(",\"humidity\":"));
  Serial.print(humidity, 1);
  Serial.print(F(",\"pump\":"));
  Serial.print(pumpState ? 1 : 0);
  Serial.print(F(",\"tank\":"));
  Serial.print(reservoirOk ? "\"OK\"" : "\"LOW\"");
  Serial.println(F("}"));
}
