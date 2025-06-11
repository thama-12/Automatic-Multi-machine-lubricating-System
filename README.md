# Automatic-Multi-machine-lubricating-System
Designed and led the development of a centralized lubrication system using a petrol engine and solenoid valve to automatically lubricate multiple machines. Improved efficiency and reduced manual intervention. Role: Mechanical Lead & CAD Designer.
/*
  -------------------------------------------------------------------
  Title      : Automatic Multi-Machine Lubrication System
  Author     : Thamarai Kannan M K S
  Created on : June 2025
  Platform   : Arduino Uno
  Version    : 1.0

  Description:
    This system monitors the temperature of multiple machines 
    (e.g., lathe, milling, grinding, shaper) and activates a 
    lubrication system when the temperature of any machine 
    exceeds a defined threshold.

    Each machine is monitored via a dedicated LM35 temperature sensor, 
    and a corresponding solenoid valve is used to control lubrication.

  -------------------------------------------------------------------

  Components Used:
    - Arduino Uno
    - LM35 Temperature Sensors (Analog)
    - Solenoid Valves (Controlled via relay or MOSFET)
    - External Power Supply for Valves
    - Relay Module or MOSFET Switches

  -------------------------------------------------------------------

  Pin Configuration:

    Machine     Temp Sensor Pin    Valve Control Pin
    --------    ----------------   -----------------
    Lathe       A0                 D4
    Milling     A1                 D5
    Grinding    A2                 D6
    Shaper      A3                 D7

  -------------------------------------------------------------------
*/

// ----------------------- Configuration Parameters -----------------------

const int NUM_MACHINES = 4;                      // Total number of machines
const int tempSensorPins[NUM_MACHINES] = {A0, A1, A2, A3};  // Analog pins for LM35 sensors
const int valvePins[NUM_MACHINES] = {4, 5, 6, 7};           // Digital pins to control solenoid valves

const float TEMP_THRESHOLD = 45.0;               // Temperature threshold in °C
const unsigned long LUBRICATION_DURATION = 5000; // Lubrication duration in ms (e.g., 5 seconds)

// ----------------------- State Tracking Arrays -----------------------

bool valveActive[NUM_MACHINES];                 // Tracks if valve is currently on
unsigned long lastActivationTime[NUM_MACHINES]; // Stores time when valve was activated

// ----------------------------- Setup Function -----------------------------

void setup() {
  Serial.begin(9600); // Start serial communication

  for (int i = 0; i < NUM_MACHINES; i++) {
    pinMode(valvePins[i], OUTPUT);             // Set valve pin as output
    digitalWrite(valvePins[i], LOW);           // Ensure valve starts OFF
    valveActive[i] = false;                    // Initially valves are off
    lastActivationTime[i] = 0;                 // No activation time yet
  }

  Serial.println("System Initialized: Monitoring temperatures...");
}

// ----------------------------- Main Loop ----------------------------------

void loop() {
  unsigned long currentTime = millis(); // Get current time

  for (int i = 0; i < NUM_MACHINES; i++) {
    float temperatureC = readTemperature(tempSensorPins[i]); // Get temp in Celsius

    Serial.print("Machine ");
    Serial.print(i + 1);
    Serial.print(" Temperature: ");
    Serial.print(temperatureC);
    Serial.println(" °C");

    // Turn on valve if temperature is above threshold
    if (temperatureC >= TEMP_THRESHOLD && !valveActive[i]) {
      activateValve(i, currentTime);
    }

    // Turn off valve after duration
    if (valveActive[i] && (currentTime - lastActivationTime[i] >= LUBRICATION_DURATION)) {
      deactivateValve(i);
    }
  }

  delay(500); // Short delay for stability
}

// ------------------------- Function: Read Temperature ---------------------

float readTemperature(int pin) {
  int sensorValue = analogRead(pin);               // Read analog voltage
  float voltage = sensorValue * (5.0 / 1023.0);     // Convert to voltage
  float tempC = (voltage - 0.5) * 100.0;            // LM35 formula
  return tempC;
}

// ------------------------ Function: Activate Valve ------------------------

void activateValve(int index, unsigned long currentTime) {
  digitalWrite(valvePins[index], HIGH);            // Turn on solenoid valve
  valveActive[index] = true;                       // Update state
  lastActivationTime[index] = currentTime;         // Store activation time

  Serial.print("→ Lubrication STARTED for Machine ");
  Serial.println(index + 1);
}

// ------------------------ Function: Deactivate Valve ----------------------

void deactivateValve(int index) {
  digitalWrite(valvePins[index], LOW);             // Turn off solenoid valve
  valveActive[index] = false;                      // Update state

  Serial.print("✓ Lubrication STOPPED for Machine ");
  Serial.println(index + 1);
}
