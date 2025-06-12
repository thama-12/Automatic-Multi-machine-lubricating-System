/*
  ========================================================================
  Project Title   : Automatic Multi-Machine Lubrication System
  Author          : Thamarai Kannan M K S
  Institution     : B.E Mechatronics Engineering
  Created Date    : June 2025
  Platform        : Arduino Uno
  Version         : 1.0

  Description:
    This system is designed to automatically lubricate multiple machines
    (Lathe, Milling, Grinding, Shaper) in a shop floor environment using
    temperature-based control logic. LM35 sensors are used to monitor 
    machine temperature. When a machine exceeds a preset temperature 
    threshold, the corresponding solenoid valve is triggered for a 
    fixed lubrication duration.

  Hardware Components:
    - 4 × LM35 Analog Temperature Sensors
    - 4 × Solenoid Valves (connected via Relay or MOSFET Driver)
    - Pneumatic/Hydraulic Cylinders (mechanical system)
    - Hollow Shaft Lubrication Line (mechanical system)
    - External 12V/24V Power Supply (for solenoids)
    - Arduino Uno

  Pin Configuration:
    Machine       | Temp Sensor Pin | Solenoid Control Pin
    ------------- | --------------- | ---------------------
    Lathe         | A0              | D4
    Milling       | A1              | D5
    Grinding      | A2              | D6
    Shaper        | A3              | D7

  ========================================================================
*/

// ----------------------- USER CONFIGURATIONS ------------------------

const uint8_t NUM_MACHINES = 4;  // Total machines being monitored

const uint8_t TEMP_SENSOR_PINS[NUM_MACHINES] = {A0, A1, A2, A3};  // LM35 inputs
const uint8_t VALVE_PINS[NUM_MACHINES] = {4, 5, 6, 7};             // Solenoid outputs

const float TEMP_THRESHOLD_C = 45.0;                // Temperature threshold (°C)
const unsigned long VALVE_ACTIVE_DURATION_MS = 5000; // Lubrication time (ms)

// ----------------------- STATE TRACKING ----------------------------

bool valveStatus[NUM_MACHINES] = {false};              // Valve ON/OFF status
unsigned long valveStartTime[NUM_MACHINES] = {0};      // Time when valve was activated

// ------------------------ ARDUINO SETUP ----------------------------

void setup() {
  Serial.begin(9600);

  // Initialize solenoid valve pins
  for (uint8_t i = 0; i < NUM_MACHINES; i++) {
    pinMode(VALVE_PINS[i], OUTPUT);
    digitalWrite(VALVE_PINS[i], LOW);  // Ensure valves start OFF
  }

  Serial.println(F("System Initialized: Monitoring started..."));
}

// ------------------------- MAIN LOOP -------------------------------

void loop() {
  unsigned long currentMillis = millis();

  for (uint8_t i = 0; i < NUM_MACHINES; i++) {
    float tempC = readTemperatureC(TEMP_SENSOR_PINS[i]);

    Serial.print(F("Machine "));
    Serial.print(i + 1);
    Serial.print(F(" → Temperature: "));
    Serial.print(tempC);
    Serial.println(F(" °C"));

    // Condition: Temperature exceeds threshold AND valve is OFF
    if (tempC >= TEMP_THRESHOLD_C && !valveStatus[i]) {
      digitalWrite(VALVE_PINS[i], HIGH);
      valveStatus[i] = true;
      valveStartTime[i] = currentMillis;

      Serial.print(F("✅ Lubrication STARTED for Machine "));
      Serial.println(i + 1);
    }

    // Condition: Valve ON for specified duration → Turn OFF
    if (valveStatus[i] && (currentMillis - valveStartTime[i] >= VALVE_ACTIVE_DURATION_MS)) {
      digitalWrite(VALVE_PINS[i], LOW);
      valveStatus[i] = false;

      Serial.print(F("🛑 Lubrication STOPPED for Machine "));
      Serial.println(i + 1);
    }
  }

  delay(500); // System stability delay
}

// -------------------- TEMPERATURE READING FUNCTION ------------------

/**
 * Reads temperature in Celsius from LM35 sensor
 * @param pin Analog pin connected to LM35
 * @return Temperature in Celsius
 */
float readTemperatureC(uint8_t pin) {
  int rawADC = analogRead(pin);
  float voltage = rawADC * (5.0 / 1023.0);          // Convert to volts
  float temperatureC = (voltage - 0.5) * 100.0;      // LM35 calculation
  return temperatureC;
}
